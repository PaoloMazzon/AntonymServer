/// \file Util.h
/// \author Paolo Mazzon
/// \brief Defines some utility features like memory and logging
#pragma once
#include "NymS/Structs.h"

/// \brief Same as malloc but crashes on failure (only use nymSFree for this data)
void *nymSMalloc(uint32_t size);

/// \brief Same as malloc but crashes on failure and zeroes the data (only use nymSFree for this data)
void *nymSCalloc(uint32_t size);

/// \brief Same as realloc but crashes on failure (only use nymSFree)
void *nymSRealloc(void *ptr, uint32_t size);

/// \brief Same as free but in the Nym engine (NULL is safe to pass)
void nymSFree(void *ptr);

/// \brief Logs a message to the debug console and to a file
void nymSLog(NymSLogLevel level, const char *fmt, ...);

/// \brief Same as above but does not log to a file
void nymSPrint(const char *fmt, ...);

/// \brief Makes sure a string is safe by putting a \0 at the end of a string
/// \param string String to sanitize
/// \param size Max size in bytes the string is supposed to be
void nymSSanitizeString(char *string, int size);
