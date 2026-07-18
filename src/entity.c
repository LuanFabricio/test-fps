#include "entity.h"

#include "raymath.h"

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
