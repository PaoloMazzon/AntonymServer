/// \file Game.c
/// \author Paolo Mazzon
#include "NymS/Game.h"
#include "NymS/Server.h"
#include "NymS/Command.h"
#include "NymS/Util.h"
#include "NymS/Client.h"

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
					nymSPrint("  - %s [%i] from host %s\n", list[i]->playerName, list[i]->id, list[i]->hostname);
				}
			}
			nymSClientUnlock(server);
		} else if (command->type == NYMS_COMMAND_KICK) {
			nymSPrint("Command not yet implemented.\n");
		} else if (command->type == NYMS_COMMAND_SAY) {
			nymSPrint("Command not yet implemented.\n");
		} else if (command->type == NYMS_COMMAND_SILENCE) {
			nymSPrint("Command not yet implemented.\n");
		}

		nymSCommandDestroy(command);
		command = nymSDispatcherGet(server->Shared.dispatcher);
	}

	// TODO: Send out player data when necessary
}