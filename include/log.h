#pragma once

#include "stdio.h"
#include "raylib.h"
#include <stdarg.h>

void _log_vector3(const Vector3 vec, const char* alias);

typedef enum {
	LOG_LABEL_INFO,
	LOG_LABEL_WARNING,
	LOG_LABEL_ERROR
} log_label_e;

#define log_label_format(label) "["label"]"

const char* log_label_cstr(const log_label_e label);

#define log_vector3(vec) _log_vector3(vec, #vec)
#define log_format(file, label, ...) \
	do {\
		fprintf(file, "[%s|%s:%d]: ", log_label_cstr(label), __FILE__, __LINE__); \
		fprintf(file, __VA_ARGS__); \
	} while(0)
