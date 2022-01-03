/// \file ServerThread.c
/// \author Paolo Mazzon
#include <stdio.h>
#define ENET_IMPLEMENTATION
#include "enet.h"
#include "NymS/Server.h"
#include "NymS/Util.h"
#include "NymS/Constants.h"
#include "NymS/Game.h"
#include "Nym/Packet.h"
#include "NymS/Client.h"

void nymSServerBroadcast(NymSServer server, void *data, uint32_t size, bool reliable) {
	ENetPacket *packet = enet_packet_create(data, size, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
	enet_host_broadcast(server->Server.host, 0, packet);
}

void nymSServerSendPacket(NymSServer server, void *data, uint32_t size, bool reliable, ENetPeer *peer) {
	ENetPacket *packet = enet_packet_create(data, size, reliable ? ENET_PACKET_FLAG_RELIABLE : 0);
	enet_peer_send(peer, 0, packet);
}

void nymSServerHandlePacket(NymSServer server, NymPacketClientMaster *packet, ENetPeer *peer) {
	if (packet->type == NYM_PACKET_TYPE_CLIENT_MESSAGE) {
		// Create a proper chat message
		NymSClient client = nymSClientLock(server, peer);
		if (!client->silenced) {
			char chat[1024];
			nymSSanitizeString(packet->message.message, NYM_MAX_CHAT_CHARACTERS);
			snprintf(chat, 1024, "%s: %s", client->playerName, packet->message.message);
			nymSClientUnlock(server);

			// Display it in the console
			nymSPrint("[Chat] %s\n", chat);

			// Create the packet and send it to everyone
			NymPacketServerMessage message = {NYM_PACKET_TYPE_SERVER_MESSAGE};
			strncpy(message.message, chat, NYM_MAX_CHAT_CHARACTERS);
			message.len = strlen(message.message);
			nymSServerBroadcast(server, &message, sizeof(struct NymPacketServerMessage), true);
		}
	} else if (packet->type == NYM_PACKET_TYPE_CLIENT_LOBBY) {
		// If the client's version doesn't match boot em
		if (packet->lobby.version != NYM_NET_VERSION) {
			nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Client disconnected due to version mismatch (server is [%i] client was [%i])", NYM_NET_VERSION, packet->lobby.version);
			enet_peer_disconnect(peer, 0);
		} else {
			// Copy the client's information into their client struct
			NymSClient client = nymSClientLock(server, peer);
			nymSSanitizeString(packet->lobby.playerName, NYM_NAME_MAX_CHARACTERS);
			strncpy(client->playerName, packet->lobby.playerName, NYM_NAME_MAX_CHARACTERS);
			nymSClientUnlock(server);
		}
	}
}

void nymSServerHandleConnection(NymSServer server, ENetPeer *peer) {
	NymSClient client = nymSClientCreateLocked(server, peer);
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "A new client [%i] connected from %s\n", client->id, client->hostname);

	// Send the client its id
	NymPacketServerInitial initial = {NYM_PACKET_TYPE_SERVER_INITIAL, client->id, NYM_NET_VERSION, 0, 0};
	nymSServerSendPacket(server, &initial, sizeof(struct NymPacketServerInitial), true, peer);
	client->peer = peer;

	// Send packet about new connection to all connected clients
	NymPacketServerConnection connection = {
			NYM_PACKET_TYPE_SERVER_CONNECTION,
			NYM_CONNECT_NEWCONNECTION,
			client->id,
			0, // TODO: Give an x and y
			0
	};
	nymSServerBroadcast(server, &connection, sizeof(struct NymPacketServerConnection), true);

	nymSClientUnlock(server);
}

void nymSServerHandleDisconnect(NymSServer server, ENetPeer *peer, bool timeout) {
	// Show that the client dc'd in the console
	NymSClient client = nymSClientLock(server, peer);
	if (client != NULL) {
		nymSLog(NYMS_LOG_LEVEL_MESSAGE, "%s [%i] disconnected.\n", client->hostname, client->id);

		// Send a message in chat about it
		char chat[1024];
		snprintf(chat, 1024, "%s disconnected.", client->playerName);
		NymPacketServerMessage message = {NYM_PACKET_TYPE_SERVER_MESSAGE};
		strncpy(message.message, chat, NYM_MAX_CHAT_CHARACTERS);
		message.len = strlen(message.message);
		nymSServerBroadcast(server, &message, sizeof(struct NymPacketServerMessage), true);

		// Send packet about new connection to all connected clients
		NymPacketServerConnection connection = {
				NYM_PACKET_TYPE_SERVER_CONNECTION,
				timeout ? NYM_CONNECT_DISCONNECTED_TIMEOUT : NYM_CONNECT_DISCONNECTED_NORMAL,
				client->id,
				0,
				0
		};
		nymSServerBroadcast(server, &connection, sizeof(struct NymPacketServerConnection), true);

		// Delete the client
		nymSClientUnlock(server);
		nymSClientDestroy(server, peer);
	}
}

void nymSServerHandleEvents(NymSServer server) {
	ENetEvent event;
	while (enet_host_service(server->Server.host, &event, 0) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				// Print new client information
				event.peer->data = "Client information";
				nymSServerHandleConnection(server, event.peer);
				break;
			case ENET_EVENT_TYPE_RECEIVE:;
				// Get the packet and pass it off to the packet handling function
				NymPacketClientMaster packet = {};
				if (sizeof(struct NymPacketClientMaster) - NYM_PACKET_HEADER_OFFSET < event.packet->dataLength) {
					nymSLog(NYMS_LOG_LEVEL_WARNING, "Bad packet of size %i received.", event.packet->dataLength);
				} else {
					memcpy(&packet.message, event.packet->data, event.packet->dataLength);
					packet.type = packet.message.type;
					nymSServerHandlePacket(server, &packet, event.peer);
				}
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				nymSServerHandleDisconnect(server, event.peer, false);
			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
				nymSServerHandleDisconnect(server, event.peer, true);

				/* Reset the peer's client information. */
				event.peer -> data = NULL;

		}
	}
}

void *nymSServer(void *data) {
	NymSServer server = data;
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Starting server thread...");
	server->Shared.serverRunning = true;
	if (enet_initialize() != 0)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to initialize enet.");

	// Start server
	ENetAddress address = {ENET_HOST_ANY, NYMS_DEFAULT_PORT};
	server->Server.host = enet_host_create(&address, NYMS_MAX_CLIENTS, 2, 0, 0);
	if (server->Server.host == NULL)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to initialize server.");

	// Server equivalent game loop
	while (server->Shared.status != NYMS_THREAD_STATUS_QUIT) {
		nymSServerHandleEvents(server);
		nymSGameProcess(server);
	}

	// Close server
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Quitting server thread...");
	enet_host_destroy(server->Server.host);
	enet_deinitialize();
	server->Shared.serverRunning = false;
	server->Shared.status = NYMS_THREAD_STATUS_QUIT;
	return NULL;
}