#pragma once

#include "raylib.h"

void _log_vector3(const Vector3 vec, const char* alias);

#define log_vector3(vec) _log_vector3(vec, #vec)
