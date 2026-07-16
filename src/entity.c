#include "entity.h"

#include "raymath.h"

entity_t entity_create(Vector3 position, Vector3 size, Color color)
{
	return (entity_t){
		.position = position,
		.size = size,
		.hitbox = {
			.center = position,
			.size = size,
		},
		.color = color,
		.attributes = {314},
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
