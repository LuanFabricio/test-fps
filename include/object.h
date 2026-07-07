#pragma once

#include "general.h"

typedef struct {
	Vector3 position, size;
	Vector3 hitbox_position, hitbox_size;
	Color color;
} object_create_params;

object_t _object_create(object_create_params params);
#define object_create(...) _object_create((object_create_params) {__VA_ARGS__})
void object_move_position(object_t *obj, const Vector3 move);
void object_update_position(object_t *obj, const Vector3 position);
