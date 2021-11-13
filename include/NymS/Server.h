/// \file Server.h
/// \author Paolo Mazzon
/// \brief Top level server functions
#pragma once
#include <pthread.h>
#include "NymS/Structs.h"

struct NymSServer {
	_Atomic uint32_t status; ///< Cross thread status codes
	pthread_t serverThread;  ///< Thread the server is in
};

/// \brief Stars the server
void nymSStart();

/// \brief The actual server
void *nymSServer(void *data);