#include "log.h"

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
