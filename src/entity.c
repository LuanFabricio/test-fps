#include <stddef.h>
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#include "dynamic_array.h"

#include "entity.h"

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
	da_append(&data->entities, entity);
	da_append(&data->velocity, velocity);
	da_append(&data->billboard_textures, (Texture2D){});
	entities_data_update_billboard_texture(data, data->entities.size-1);
}

void entities_data_remove(entities_data_t *data, const size_t i)
{
	da_remove(&data->entities, i);
	da_remove(&data->velocity, i);
	UnloadTexture(data->billboard_textures.items[i]);
	da_remove(&data->billboard_textures, i);
}

void entities_data_update_billboard_texture(entities_data_t *data, const size_t i)
{
	entity_t entity = data->entities.items[i];
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

	UnloadTexture(data->billboard_textures.items[i]);
	data->billboard_textures.items[i] = LoadTextureFromImage(img);
	UnloadImage(img);
}
