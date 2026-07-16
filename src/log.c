#include "log.h"

#include "macros/utils.h"

void _log_vector3(const Vector3 vec, const char* alias)
{
		TraceLog(
			LOG_INFO,
			"%s = {\n"
			"\t.x = %f\n"
			"\t.y = %f\n"
			"\t.z = %f\n"
			"}",
			alias,
			vec.x,
			vec.y,
			vec.z
		);
}

const char* log_label_cstr(const log_label_e label)
{
	switch (label) {
		case LOG_LABEL_INFO:
			return "INFO";
		case LOG_LABEL_WARNING:
			return "WARNING";
		case LOG_LABEL_ERROR:
			return "ERROR";
	}
	UNREACHABLE("Invalid label");
}
