#pragma once

#include "raylib.h"

#include "shapes.h"
#include "system/attributes.h"

typedef struct {
	Vector3 position, size;

	cube_t hitbox;

	Color color;
	attributes_t attributes;
} entity_t;

entity_t entity_create(Vector3 position, Vector3 size, Color color);
void entity_move_position(entity_t *entity, Vector3 move);
void entity_update_position(entity_t *entity, Vector3 position);
