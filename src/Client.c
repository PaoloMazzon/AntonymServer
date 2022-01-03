/// \file Client.c
/// \author Paolo Mazzon
#include "NymS/Client.h"
#include "NymS/Server.h"
#include "NymS/Util.h"

NymSClient nymSClientCreateLocked(NymSServer server, ENetPeer *peer) {
	int spot = -1;
	pthread_mutex_lock(&server->Shared.clientMutex);
	for (int i = 0; i < NYMS_MAX_CLIENTS && spot == -1; i++)
		if (server->Shared.clients[i] == NULL)
			spot = i;

	if (spot != -1) {
		server->Shared.clients[spot] = nymSMalloc(sizeof(struct NymSClient));
		enet_address_get_host_ip_old(&peer->address, server->Shared.clients[spot]->hostname, NYMS_MAX_HOST_CHARACTERS);
		server->Shared.clients[spot]->address = peer->address;
		server->Shared.clients[spot]->id = server->Server.idGenerator++;
		server->Shared.clients[spot]->silenced = false;
		return server->Shared.clients[spot];
	} else {
		nymSLog(NYMS_LOG_LEVEL_ERROR, "Client attempted to connect past max.");
	}
	return NULL;
}

void nymSClientCreate(NymSServer server, ENetPeer *peer) {
	nymSClientCreateLocked(server, peer);
	pthread_mutex_unlock(&server->Shared.clientMutex);
}

NymSClient nymSClientLock(NymSServer server, ENetPeer *peer) {
	pthread_mutex_lock(&server->Shared.clientMutex);
	for (int i = 0; i < NYMS_MAX_CLIENTS; i++)
		if (server->Shared.clients[i] != NULL && memcmp(&server->Shared.clients[i]->address, &peer->address, sizeof(ENetAddress)) == 0)
			return server->Shared.clients[i];
	return NULL;
}

NymSClient *nymSClientGetList(NymSServer server, int *size) {
	pthread_mutex_lock(&server->Shared.clientMutex);
	*size = NYMS_MAX_CLIENTS;
	return server->Shared.clients;
}

void nymSClientUnlock(NymSServer server) {
	pthread_mutex_unlock(&server->Shared.clientMutex);
}

void nymSClientDestroy(NymSServer server, ENetPeer *peer) {
	int spot;
	pthread_mutex_lock(&server->Shared.clientMutex);
	for (int i = 0; i < NYMS_MAX_CLIENTS && spot == -1; i++)
		if (server->Shared.clients[i] != NULL && memcmp(&server->Shared.clients[i]->address, &peer->address, sizeof(ENetAddress)) == 0)
			spot = i;
	nymSFree(server->Shared.clients[spot]);
	server->Shared.clients[spot] = NULL;
	pthread_mutex_unlock(&server->Shared.clientMutex);
}