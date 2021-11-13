/// \file ServerThread.c
/// \author Paolo Mazzon
#define ENET_IMPLEMENTATION
#include "enet.h"
#include "NymS/Server.h"
#include "NymS/Util.h"
#include "NymS/Constants.h"
#include "NymS/Game.h"

void nymSServerHandleEvents(NymSServer server) {
	ENetEvent event;
	while (enet_host_service(server->Server.host, &event, 0) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				nymSLog(NYMS_LOG_LEVEL_MESSAGE, "A new client connected from %x:%u.\n",
						event.peer -> address.host,
						event.peer -> address.port);
				/* Store any relevant client information here. */
				event.peer -> data = "Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				nymSLog(NYMS_LOG_LEVEL_MESSAGE, "A packet of length %u containing %s was received from %s on channel %u.\n",
						event.packet -> dataLength,
						event.packet -> data,
						event.peer -> data,
						event.channelID);
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy (event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				nymSLog(NYMS_LOG_LEVEL_MESSAGE, "%s disconnected.\n", event.peer->data);

				break;

			case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
				nymSLog(NYMS_LOG_LEVEL_MESSAGE, "%s disconnected.\n", event.peer->data);

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