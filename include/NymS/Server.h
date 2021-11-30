/// \file Server.h
/// \author Paolo Mazzon
/// \brief Top level server functions
#pragma once
#include <pthread.h>
#include "enet.h"
#include "NymS/Structs.h"
#include "NymS/Constants.h"

/// \brief Information the server needs to run
struct NymSServer {
	///< Data that is shared between the two threads
	struct {
		_Atomic uint32_t status;              ///< Cross thread status codes
		_Atomic bool serverRunning;           ///< Tells the main thread if the server thread is active or not
		NymSClient clients[NYMS_MAX_CLIENTS]; ///< List of all connected clients
		pthread_mutex_t clientMutex;          ///< Mutex for accessing the clients list
		NymSDispatcher dispatcher;            ///< Command dispatcher
	} Shared;

	///< Data exclusive to the REPL thread
	struct {
		pthread_t serverThread; ///< Thread the server is in
	} Local;

	///< Data exclusively for the server thread
	struct {
		ENetHost *host;  ///< Internal enet host
		int idGenerator; ///< Incremented to generate new client ids
	} Server;
};

/// \brief Stars the server
void nymSStart();

/// \brief The actual server
void *nymSServer(void *data);