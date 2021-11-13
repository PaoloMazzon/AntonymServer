/// \file REPL.c
/// \author Paolo Mazzon
#include <stdio.h>
#include "NymS/REPL.h"
#include "NymS/Util.h"

void nymSREPLProcess(NymSServer server) {
	// Get line
	char input[1024];
	fflush(stdin);
	nymSPrint("Server> ");
	scanf("%1023[^\n]", input);

	// Process line
	if (strcmp(input, "quit") == 0) {
		server->Shared.status = NYMS_THREAD_STATUS_QUIT;
	} else {
		nymSPrint("Unrecognized command \"%s\"\n", input);
	}
}