/// \file Structs.h
/// \author Paolo Mazzon
/// \brief Forward declares things
#pragma once
#include <stdint.h>

/*********************** Forward Declare Structs ***********************/
typedef struct NymSServer *NymSServer;

/*********************** Enums ***********************/

/// \brief Level of severity for logs
typedef enum {
	NYMS_LOG_LEVEL_MESSAGE = 0,  ///< Just a message
	NYMS_LOG_LEVEL_WARNING = 1,  ///< Warning something has gone wrong
	NYMS_LOG_LEVEL_ERROR = 2,    ///< Recoverable error
	NYMS_LOG_LEVEL_CRITICAL = 3, ///< Fatal error that will crash now the game
} NymSLogLevel;