/// \file ServerThread.c
/// \author Paolo Mazzon
#define ENET_IMPLEMENTATION
#include "enet.h"
#include "NymS/Server.h"
#include "NymS/Util.h"
#include "NymS/Constants.h"

void *nymSServer(void *data) {
	NymSServer server = data;
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Starting server thread...");
	if (enet_initialize() != 0)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to initialize enet.");

	// Start server
	ENetAddress address = {ENET_HOST_ANY, NYMS_DEFAULT_PORT};
	ENetHost *host = enet_host_create(&address, NYMS_MAX_CLIENTS, 2, 0, 0);
	if (host == NULL)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to initialize server.");

	// Manage events
	// TODO: This

	// Close server
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Quitting server thread...");
	enet_host_destroy(host);
	enet_deinitialize();
	server->status = NYMS_THREAD_STATUS_QUIT;
	return NULL;
}