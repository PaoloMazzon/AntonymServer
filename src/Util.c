/// \file Util.c
/// \author Paolo Mazzon
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "NymS/Util.h"
#include "NymS/Constants.h"

// For making log thread safe
_Atomic int gLogInUse = 0;

void *nymSMalloc(uint32_t size) {
	void *out = malloc(size);
	if (out == NULL)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to allocate %i bytes", size);
	return out;
}

void *nymSCalloc(uint32_t size) {
	void *out = calloc(1, size);
	if (out == NULL)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to allocate %i bytes", size);
	return out;
}

void *nymSRealloc(void *ptr, uint32_t size) {
	void *out = realloc(ptr, size);
	if (out == NULL)
		nymSLog(NYMS_LOG_LEVEL_CRITICAL, "Failed to reallocate %i bytes", size);
	return out;
}

void nymSFree(void *ptr) {
	if (ptr != NULL) free(ptr);
}

void nymSLog(NymSLogLevel level, const char *fmt, ...) {
	va_list list;

	// Wait till the function is available
	while (gLogInUse) {
		volatile int i = 0;
	}
	gLogInUse = 1;

	// To stdout
	va_start(list, fmt);
	if (level == NYMS_LOG_LEVEL_MESSAGE)
		printf("[Nym Message] ");
	else if (level == NYMS_LOG_LEVEL_WARNING)
		printf("[Nym Warning] ");
	else if (level == NYMS_LOG_LEVEL_ERROR)
		printf("[Nym Error] ");
	else if (level == NYMS_LOG_LEVEL_CRITICAL)
		printf("[Nym Fatal Error] ");
	vprintf(fmt, list);
	printf("\n");
	fflush(stdout);
	va_end(list);

	// To file
	FILE *file = fopen(NYMS_LOG_FILE, "a");
	va_start(list, fmt);
	if (level == NYMS_LOG_LEVEL_MESSAGE)
		fprintf(file, "[Nym Message] ");
	else if (level == NYMS_LOG_LEVEL_WARNING)
		fprintf(file, "[Nym Warning] ");
	else if (level == NYMS_LOG_LEVEL_ERROR)
		fprintf(file, "[Nym Error] ");
	else if (level == NYMS_LOG_LEVEL_CRITICAL)
		fprintf(file, "[Nym Fatal Error] ");
	vfprintf(file, fmt, list);
	fprintf(file, "\n");
	fclose(file);
	va_end(list);

	gLogInUse = 0;

	// Abort if critical error
	if (level == NYMS_LOG_LEVEL_CRITICAL)
		abort();
}

void nymSPrint(const char *fmt, ...) {
	va_list list;

	// Wait till the function is available
	while (gLogInUse) {
		volatile int i = 0;
	}
	gLogInUse = 1;

	// To stdout
	va_start(list, fmt);
	vprintf(fmt, list);
	fflush(stdout);
	va_end(list);

	gLogInUse = 0;
}

void nymSSanitizeString(char *string, int size) {
	string[size - 1] = 0;
}