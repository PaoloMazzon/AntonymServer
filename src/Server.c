/// \file Server.c
/// \author Paolo Mazzon
#include "NymS/REPL.h"
#include "NymS/Server.h"
#include "NymS/Util.h"

void nymSStart() {
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Starting server...");
	NymSServer server = nymSCalloc(sizeof(struct NymSServer));

	// Create the client mutex
	pthread_mutexattr_t mut;
	pthread_mutexattr_init(&mut);
	pthread_mutexattr_setprotocol(&mut, PTHREAD_MUTEX_DEFAULT);
	pthread_mutex_init(&server->Shared.clientMutex, &mut);

	// Start the server thread
	server->Shared.status = NYMS_THREAD_STATUS_OK;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&server->Local.serverThread, &attr, nymSServer, server) != 0)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to create server thread.");

	// Wait till the server thread has started to begin the repl
	while (!server->Shared.serverRunning) {
		volatile int yup = 0;
	}

	// Wait till we get the exit status to quit
	while (server->Shared.status != NYMS_THREAD_STATUS_QUIT) {
		nymSREPLProcess(server);
	}

	// Wait till the server thread has stopped to free the memory
	while (server->Shared.serverRunning) {
		volatile int yup = 0;
	}

	nymSFree(server);
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Quitting server...");
}