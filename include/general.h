#pragma once

#include <stdbool.h>

#include "raylib.h"

#include "shapes.h"
#include "system/attributes.h"

typedef struct {
	line_t line;
	Ray ray;
	double death_time;
} ray_t;


float randf();
