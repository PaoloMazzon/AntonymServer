/// \file Game.c
/// \author Paolo Mazzon
#include <stdio.h>
#include "NymS/Game.h"
#include "NymS/Server.h"
#include "NymS/Command.h"
#include "NymS/Util.h"
#include "NymS/Client.h"
#include "Nym/Packet.h"

void nymSServerBroadcast(NymSServer server, void *data, uint32_t size, bool reliable);
void nymSGameProcess(NymSServer server) {
	// Process commands
	NymSCommand command = nymSDispatcherGet(server->Shared.dispatcher);
	while (command != NULL) {
		if (command->type == NYMS_COMMAND_LIST) {
			// Print info on all connected players
			nymSPrint("Connected Players:\n");
			int size;
			NymSClient *list = nymSClientGetList(server, &size);
			for (int i = 0; i < size; i++) {
				if (list[i] != NULL) {
					if (list[i]->silenced)
						nymSPrint("  - %s [%i] from host %s (silenced)\n", list[i]->playerName, list[i]->id, list[i]->hostname);
					else
						nymSPrint("  - %s [%i] from host %s\n", list[i]->playerName, list[i]->id, list[i]->hostname);
				}
			}
			nymSClientUnlock(server);
		} else if (command->type == NYMS_COMMAND_KICK) {
			// Find the targeted client
			int id, size;
			ENetPeer *peer = NULL;
			id = strtol(command->argument, NULL, 10);
			NymSClient *list = nymSClientGetList(server, &size);
			for (int i = 0; i < size; i++)
				if (list[i] != NULL && list[i]->id == id)
					peer = server->Shared.clients[i]->peer;

			if (peer != NULL) {
				enet_peer_disconnect(peer, NYM_NET_KICK_DATA);
				nymSPrint("Player ID [%i] kicked.\n", id);
			} else {
				nymSPrint("Player of ID [%i] does not exist.\n", id);
			}
			nymSClientUnlock(server);
		} else if (command->type == NYMS_COMMAND_SAY) {
			// Create the packet and send it to everyone
			NymPacketServerMessage message = {NYM_PACKET_TYPE_SERVER_MESSAGE};
			strncpy(message.message, command->argument, NYM_MAX_CHAT_CHARACTERS);
			message.len = strlen(message.message);
			nymSServerBroadcast(server, &message, sizeof(struct NymPacketServerMessage), true);
		} else if (command->type == NYMS_COMMAND_SILENCE) {
			// Find the targeted client
			int id, size;
			NymSClient client = NULL;
			id = strtol(command->argument, NULL, 10);
			NymSClient *list = nymSClientGetList(server, &size);
			for (int i = 0; i < NYMS_MAX_CLIENTS; i++)
				if (list[i] != NULL && list[i]->id == id)
					client = server->Shared.clients[i];

			if (client != NULL) {
				client->silenced = !client->silenced;
				if (client->silenced)
					nymSPrint("Player ID [%i] silenced.\n", id);
				else
					nymSPrint("Player ID [%i] unsilenced.\n", id);
			} else {
				nymSPrint("Player of ID [%i] does not exist.\n", id);
			}
			nymSClientUnlock(server);
		}

		nymSCommandDestroy(command);
		command = nymSDispatcherGet(server->Shared.dispatcher);
	}

	// TODO: Send out player data when necessary
}