/// \file Game.h
/// \author Paolo Mazzon
/// \brief Declares functionality for keeping track of the game at the highest level
#pragma once
#include "NymS/Structs.h"

/// \brief Processes the game (intended for the server thread)
void nymSGameProcess(NymSServer server);