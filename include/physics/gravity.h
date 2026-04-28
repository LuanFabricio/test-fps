#pragma once

#include <stddef.h>

#include "shapes.h"

#define HITBOXES_MAX_CAPACITY 20
typedef struct {
	cube_t items[HITBOXES_MAX_CAPACITY];
	size_t size;
} hitboxes_t;

void gravity_apply(Vector3 *velocity, const float delta_time);
