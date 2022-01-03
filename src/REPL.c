/// \file REPL.c
/// \author Paolo Mazzon
#include <stdio.h>
#include "NymS/REPL.h"
#include "NymS/Util.h"
#include "NymS/Command.h"

void nymSREPLProcess(NymSServer server) {
	// Get line
	char input[1024];
	char *argument = ""; // Comes from input
	fflush(stdin);
	scanf("%1023[^\n]", input);

	// Find a space if it exists
	for (int i = 0; i < strlen(input) && strlen(argument) == 0; i++) {
		if (input[i] == ' ' && i < strlen(input) - 1) {
			input[i] = 0;
			argument = &(input[i + 1]);
		}
	}

	// Process the command
	if (strcmp(input, "kick") == 0) {
		nymSDispatcherQueue(server->Shared.dispatcher, nymSCommandCreate(NYMS_COMMAND_KICK, argument));
	} else if (strcmp(input, "list") == 0) {
		nymSDispatcherQueue(server->Shared.dispatcher, nymSCommandCreate(NYMS_COMMAND_LIST, argument));
	} else if (strcmp(input, "silence") == 0) {
		nymSDispatcherQueue(server->Shared.dispatcher, nymSCommandCreate(NYMS_COMMAND_SILENCE, argument));
	} else if (strcmp(input, "say") == 0) {
		nymSDispatcherQueue(server->Shared.dispatcher, nymSCommandCreate(NYMS_COMMAND_SAY, argument));
	} else if (strcmp(input, "help") == 0) {
		nymSPrint("Commands:\n  - kick [id]\n  - list\n  - silence [id]\n  - say [something]\n  - help\n  - quit\n");
	} else if (strcmp(input, "quit") == 0) {
		server->Shared.status = NYMS_THREAD_STATUS_QUIT;
	} else {
		nymSPrint("Unrecognized command \"%s\"\n", input);
	}
}