/// \file ServerThread.c
/// \author Paolo Mazzon
#include "NymS/Server.h"
#include "NymS/Util.h"

void *nymSServer(void *data) {
	NymSServer server = data;
	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Starting server thread...");



	nymSLog(NYMS_LOG_LEVEL_MESSAGE, "Quitting server thread...");
	server->status = NYMS_THREAD_STATUS_QUIT;
	return NULL;
}