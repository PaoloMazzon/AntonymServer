/// \file Server.c
/// \author Paolo Mazzon
#include <stdbool.h>
#include "NymS/Server.h"
#include "NymS/Util.h"

void nymSStart() {
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Starting server...");
	NymSServer server = nymSCalloc(sizeof(struct NymSServer));

	// Start the server thread
	server->status = NYMS_THREAD_STATUS_OK;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	int status = pthread_create(&server->serverThread, &attr, nymSServer, server);

	// Wait till we get the exit status to quit
	bool quit = false;
	while (!quit) {
		quit = server->status == NYMS_THREAD_STATUS_QUIT;
	}

	nymSFree(server);
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Quitting server...");
}