/// \file Client.h
/// \author Paolo Mazzon
/// \brief Things to manage a connected client
#pragma once
#include "enet.h"
#include "NymS/Constants.h"
#include "Nym/Structs.h"
#include "NymS/Server.h"

/// \brief Information on a given client
struct NymSClient {
	char hostname[NYMS_MAX_HOST_CHARACTERS];   ///< Hostname (ip)
	NymPlayerID id;                            ///< ID of this client
	ENetAddress address;                       ///< Address of this client
	char playerName[NYMS_NAME_MAX_CHARACTERS]; ///< Name of this client
};

/// \brief Creates a new player and puts it in a free spot in the server
void nymSClientCreate(NymSServer server, ENetPeer *peer);

/// \brief Creates a new client and returns it, locking the clients (call `nymSClientUnlock` when you're done)
NymSClient nymSClientCreateLocked(NymSServer server, ENetPeer *peer);

/// \brief Gets a client given a peer and locks the clients (call `nymSClientUnlock` when you're done) (returns NULL if client doesn't exist)
NymSClient nymSClientLock(NymSServer server, ENetPeer *peer);

/// \brief Unlocks the clients after a `nymSClientLock` call
void nymSClientUnlock(NymSServer server);

/// \brief Removes a client from the server given a peer
void nymSClientDestroy(NymSServer server, ENetPeer *peer);