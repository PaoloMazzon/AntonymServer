/// \file Constants.h
/// \author Paolo Mazzon
/// \brief Constant externs
#pragma once

///< Log file
extern const char *NYMS_LOG_FILE;

///< Default port
extern int NYMS_DEFAULT_PORT;

///< Max number of clients that can connect to the server
#define NYMS_MAX_CLIENTS 10

///< Max number of characters in a hostname
#define NYMS_MAX_HOST_CHARACTERS 30

///< Max number of characters in a player's name
#define NYMS_NAME_MAX_CHARACTERS 15

///< Max number of characters a repl command can be
#define NYMS_MAX_COMMAND_CHARACTERS 100

///< How many elements to add to a list at a time
extern int NYMS_LIST_EXTENSION;