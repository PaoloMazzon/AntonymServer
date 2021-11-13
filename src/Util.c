/// \file Util.c
/// \author Paolo Mazzon
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "NymS/Util.h"
#include "NymS/Constants.h"

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

void nymSFree(void *ptr) {
	if (ptr != NULL) free(ptr);
}

void nymSLog(NymSLogLevel level, const char *fmt, ...) {
	va_list list;

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

	// Abort if critical error
	if (level == NYMS_LOG_LEVEL_CRITICAL)
		abort();
}