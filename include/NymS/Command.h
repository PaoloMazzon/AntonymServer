/// \file Command.h
/// \author Paolo Mazzon
/// \brief Declares commands the repl sends to the server
#pragma once
#include <pthread.h>
#include "NymS/Structs.h"
#include "NymS/Constants.h"

/// \brief Information on a given command
struct NymSCommand {
	NymSCommandType type;                       ///< Type of command this is
	char argument[NYMS_MAX_COMMAND_CHARACTERS]; ///< Everything that was typed after the command
};

/// \brief Manages commands from multiple threads (a queue of commands)
struct NymSDispatcher {
	NymSCommand *commands;       ///< List of commands
	int commandCount;            ///< Number of commands in the queue
	int commandListSize;         ///< Total size of the command list
	pthread_mutex_t commandLock; ///< Nothing in this struct will be accessed without locking this
};

/// \brief Creates a command (to be passed to a dispatcher) (the string will be copied)
NymSCommand nymSCommandCreate(NymSCommandType type, const char *argument);

/// \brief Frees a command
void nymSCommandDestroy(NymSCommand command);

/// \brief Creates a dispatcher
NymSDispatcher nymSDispatcherCreate();

/// \brief Destroys a dispatcher
void nymSDispatcherDestroy(NymSDispatcher dispatcher);

/// \brief Queues a command (don't free the command) (thread-safe)
void nymSDispatcherQueue(NymSDispatcher dispatcher, NymSCommand command);

/// \brief Gets the first command from the queue (you must free the command yourself) (thread-safe)
NymSCommand nymSDispatcherGet(NymSDispatcher dispatcher);