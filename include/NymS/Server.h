/// \file Server.h
/// \author Paolo Mazzon
/// \brief Top level server functions
#pragma once
#include <pthread.h>
#include "enet.h"
#include "NymS/Structs.h"

struct NymSServer {
	///< Data that is shared between the two threads
	struct {
		_Atomic uint32_t status;    ///< Cross thread status codes
		_Atomic bool serverRunning; ///< Tells the main thread if the server thread is active or not
	} Shared;

	///< Data exclusive to the REPL thread
	struct {
		pthread_t serverThread; ///< Thread the server is in
	} Local;

	///< Data exclusively for the server thread
	struct {
		ENetHost *host;
	} Server;
};

/// \brief Stars the server
void nymSStart();

/// \brief The actual server
void *nymSServer(void *data);