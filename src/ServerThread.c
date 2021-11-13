/// \file ServerThread.c
/// \author Paolo Mazzon
#define ENET_IMPLEMENTATION
#include "enet.h"
#include "NymS/Server.h"
#include "NymS/Util.h"

void *nymSServer(void *data) {
	NymSServer server = data;
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Starting server thread...");
	if (enet_initialize() != 0)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to initialize enet.");

	// Start server
	// TODO: This

	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Quitting server thread...");
	enet_deinitialize();
	server->status = NYMS_THREAD_STATUS_QUIT;
	return NULL;
}