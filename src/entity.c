#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"

#include "entity.h"

#define ENTITIES_DATA_INITIAL_CAPACITY 5
#define ENTITIES_DATA_GROWTH_FACTOR 2

entity_t entity_create_params(entity_create_parameters_t params)
{
	return (entity_t){
		.position = params.position,
		.size = params.size,
		.hitbox = {
			.center = Vector3Add(params.position, params.hitbox_position_offset),
			.size = Vector3Add(params.size, params.hitbox_size_offset),
		},
		.color = params.color,
		.attributes = params.attributes,
	};
}

void entity_move_position(entity_t *entity, Vector3 move)
{
	entity->position = Vector3Add(entity->position, move);
	entity->hitbox.center = Vector3Add(entity->hitbox.center, move);
}

void entity_update_position(entity_t *entity, Vector3 position)
{
	const Vector3 hitbox_position_offset = Vector3Subtract(entity->position, entity->hitbox.center);

	entity->position = position;
	entity->hitbox.center = Vector3Add(hitbox_position_offset, entity->position);
}

void entities_data_append(entities_data_t *data, entity_t entity, Vector3 velocity)
{
	if (data->capacity == 0) {
		data->capacity = ENTITIES_DATA_INITIAL_CAPACITY;
		data->entities = malloc(sizeof(*data->entities) * data->capacity);
		data->velocity = malloc(sizeof(*data->velocity) * data->capacity);
		data->billboard_textures = malloc(
				sizeof(*data->billboard_textures) * data->capacity);
	} else if ((data->size + 1) >= data->capacity) {
	 	data->capacity *= ENTITIES_DATA_GROWTH_FACTOR;
		data->entities = realloc(data->entities, sizeof(*data->entities) * data->capacity);
		data->velocity = realloc(data->velocity, sizeof(*data->velocity) * data->capacity);
		data->billboard_textures = realloc(data->billboard_textures, sizeof(*data->billboard_textures) * data->capacity);
	 }

	assert(data->entities != NULL && "Could not allocate memory for `data->entities`");
	assert(data->velocity != NULL && "Could not allocate memory for `data->velocity`");
	assert(data->billboard_textures != NULL && "Could not allocate memory for `data->billboard_textures`");

	memcpy(&(data->entities[data->size]), &entity, sizeof(*data->entities));
	memcpy(&(data->velocity[data->size]), &velocity, sizeof(*data->velocity));
	entities_data_update_billboard_texture(data, data->size);
	data->size += 1;
}

void entities_data_remove(entities_data_t *data, const size_t i)
{

	if (i < data->size) {
		if (data->size > 0) { data->size--; }
		data->entities[i] = data->entities[data->size];
		data->velocity[i] = data->velocity[data->size];
		UnloadTexture(data->billboard_textures[i]);
		data->billboard_textures[i] = data->billboard_textures[data->size];
	}
}

void entities_data_update_billboard_texture(entities_data_t *data, const size_t i)
{
	entity_t entity = data->entities[i];
	char buffer[0xff] = "";
	const int billboard_font_size = 16;
	snprintf(
			buffer,
			sizeof(buffer),
			"HP: %.02f/%.02f\n"
			"DMG: %.02f\n"
			"ELM MUL: %.02f%%\n",
			entity.attributes.current_health, entity.attributes.max_health,
			entity.attributes.damage,
			entity.attributes.elemental_multiplier);
	Image img = ImageText(buffer, billboard_font_size, WHITE);

	UnloadTexture(data->billboard_textures[i]);
	data->billboard_textures[i] = LoadTextureFromImage(img);
	UnloadImage(img);
}
