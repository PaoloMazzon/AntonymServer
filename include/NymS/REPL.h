/// \file REPL.h
/// \author Paolo Mazzon
/// \brief Defines some REPL stuff
#pragma once
#include "NymS/Server.h"

/// \brief Intended to be run on a loop in the main thread to process the REPL (blocking)
void nymSREPLProcess(NymSServer server);