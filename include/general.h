#pragma once

#include <stdbool.h>

#include "raylib.h"

#include "shapes.h"
#include "dynamic_array.h"
#include "system/attributes.h"
#include "system/player_info.h"

typedef struct {
	line_t line;
	Ray ray;
	double death_time;
} ray_t;

typedef struct {
	Vector3 position;
	Vector3 size;
	Color color;
	struct {
		BoundingBox box;
		Vector3 position, size;
	} hitbox;
	attributes_t attributes;
} object_t;


float randf();
