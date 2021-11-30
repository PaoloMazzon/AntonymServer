/// \file Command.c
/// \author Paolo Mazzon
#include <string.h>
#include "NymS/Command.h"
#include "NymS/Util.h"

NymSCommand nymSCommandCreate(NymSCommandType type, const char *argument) {
	NymSCommand command = nymSMalloc(sizeof(struct NymSCommand));
	command->type = type;
	strncpy(command->argument, argument, NYMS_MAX_COMMAND_CHARACTERS - 1);
	return command;
}

void nymSCommandDestroy(NymSCommand command) {
	nymSFree(command);
}

NymSDispatcher nymSDispatcherCreate() {
	NymSDispatcher dispatcher = nymSCalloc(sizeof(struct NymSDispatcher));
	pthread_mutexattr_t mut;
	pthread_mutexattr_init(&mut);
	pthread_mutexattr_setprotocol(&mut, PTHREAD_MUTEX_DEFAULT);
	pthread_mutex_init(&dispatcher->commandLock, &mut);
	return NULL;
}

void nymSDispatcherDestroy(NymSDispatcher dispatcher) {
	pthread_mutex_destroy(&dispatcher->commandLock);
	for (int i = 0; i < dispatcher->commandCount; i++)
		nymSCommandDestroy(dispatcher->commands[i]);
	nymSFree(dispatcher->commands);
	nymSFree(dispatcher);
}

void nymSDispatcherQueue(NymSDispatcher dispatcher, NymSCommand command) {
	pthread_mutex_lock(&dispatcher->commandLock);
	// Make list bigger if not enough elements available
	if (dispatcher->commandCount < dispatcher->commandListSize) {
		dispatcher->commands = nymSRealloc(dispatcher->commands, (dispatcher->commandListSize + NYMS_LIST_EXTENSION) * sizeof(NymSCommand));
		dispatcher->commandListSize += NYMS_LIST_EXTENSION;
	}

	// Put it in the back of the queue
	dispatcher->commands[dispatcher->commandCount] = command;
	dispatcher->commandCount++;
	pthread_mutex_unlock(&dispatcher->commandLock);
}

NymSCommand nymSDispatcherGet(NymSDispatcher dispatcher) {
	NymSCommand command = NULL;
	pthread_mutex_lock(&dispatcher->commandLock);
	// Do we even have commands in the queue
	if (dispatcher->commandCount != 0) {
		// Get the first command and move everything else back
		command = dispatcher->commands[0];
		for (int i = 0; i < dispatcher->commandCount - 1; i++)
			dispatcher->commands[i] = dispatcher->commands[i + 1];
		dispatcher->commandCount--;
	}
	pthread_mutex_unlock(&dispatcher->commandLock);
	return command;
}