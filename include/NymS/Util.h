/// \file Util.h
/// \author Paolo Mazzon
/// \brief Defines some utility features like memory and logging
#pragma once
#include "NymS/Structs.h"

/// \brief Same as malloc but crashes on failure (only use nymSFree for this data)
void *nymSMalloc(uint32_t size);

/// \brief Same as malloc but crashes on failure and zeroes the data (only use nymSFree for this data)
void *nymSCalloc(uint32_t size);

/// \brief Same as free but in the Nym engine (NULL is safe to pass)
void nymSFree(void *ptr);

/// \brief Logs a message to the debug console and to a file
void nymSLog(NymSLogLevel level, const char *fmt, ...);
