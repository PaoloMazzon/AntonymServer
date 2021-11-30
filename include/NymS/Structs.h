/// \file Structs.h
/// \author Paolo Mazzon
/// \brief Forward declares things
#pragma once
#include <stdint.h>

/*********************** Forward Declare Structs ***********************/

typedef struct NymSServer *NymSServer;
typedef struct NymSPacketClient NymSPacketClient;
typedef struct NymSPacketServer NymSPacketServer;
typedef struct NymSClient *NymSClient;
typedef struct NymSCommand *NymSCommand;
typedef struct NymSDispatcher *NymSDispatcher;

/*********************** Enums ***********************/

/// \brief Level of severity for logs
typedef enum {
	NYMS_LOG_LEVEL_MESSAGE = 0,  ///< Just a message
	NYMS_LOG_LEVEL_WARNING = 1,  ///< Warning something has gone wrong
	NYMS_LOG_LEVEL_ERROR = 2,    ///< Recoverable error
	NYMS_LOG_LEVEL_CRITICAL = 3, ///< Fatal error that will crash now the game
} NymSLogLevel;

/// \brief Status codes for the threads to synchronize
typedef enum {
	NYMS_THREAD_STATUS_OK = 0,   ///< Business as usual
	NYMS_THREAD_STATUS_QUIT = 1, ///< Quit the program
} NymSThreadStatus;

/// \brief Various types of repl commands
typedef enum {
	NYMS_COMMAND_LIST = 0,    ///< List out connected players
	NYMS_COMMAND_KICK = 1,    ///< Kick a given player ID
	NYMS_COMMAND_SILENCE = 2, ///< Silence a given player ID
	NYMS_COMMAND_SAY = 3,     ///< Speak in chat
	NYMS_COMMAND_MAX = 4,     ///< Total number of avalable commands
} NymSCommandType;