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

typedef da_create(entity_t) entities_t;
typedef da_create(Vector3) entities_velocity_t;

// NOTE: This approach creates 3 dynamic arrays that have
// the same size, create unnecessary memory and make possible
// to desync the data.
// This can be compressed using pointers to `entity_t`, `Vector3`
// and `Texture`, to store the data and add `size` and `capacity`
// fields to it.
typedef struct {
	entities_t entities;
	entities_velocity_t velocity;
	// NOTE: When use `da_remove`, call `UnloadTextore` before.
	da_create(Texture) billboard_textures;
} entities_data_t;

void entities_data_append(entities_data_t *data, entity_t entity, Vector3 velocity);
void entities_data_remove(entities_data_t *data, const size_t i);
void entities_data_update_billboard_texture(entities_data_t *data, const size_t i);
