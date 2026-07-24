#pragma once

#include "dynamic_array.h"
#include "raylib.h"

#include "shapes.h"
#include "system/attributes.h"

typedef struct {
	Vector3 position, size;

	cube_t hitbox;

	Color color;
	attributes_t attributes;
} entity_t;

typedef struct {
	Vector3 position, size;
	Vector3 hitbox_position_offset, hitbox_size_offset;
	Color color;
	attributes_t attributes;
} entity_create_parameters_t;

entity_t entity_create_params(entity_create_parameters_t parameters);
#define entity_create(...) entity_create_params((entity_create_parameters_t){ __VA_ARGS__ })
void entity_move_position(entity_t *entity, Vector3 move);
void entity_update_position(entity_t *entity, Vector3 position);

typedef struct {
	entity_t *entities;
	Vector3 *velocity;
	Texture *billboard_textures;
	size_t size, capacity;
} entities_data_t;

void entities_data_append(entities_data_t *data, entity_t entity, Vector3 velocity);
void entities_data_remove(entities_data_t *data, const size_t i);
void entities_data_update_billboard_texture(entities_data_t *data, const size_t i);
