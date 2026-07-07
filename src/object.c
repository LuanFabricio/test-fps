#include "general.h"
#include "raymath.h"

#include "object.h"

object_t _object_create(object_create_params params)
{
	object_t obj = {
		.position = params.position,
		.size = params.size,
		.color = params.color,
		.hitbox = {
			.position = params.hitbox_position,
			.size = params.hitbox_size,
			.box = {{0}},
		},
		.attributes = {
			.max_health = 10 + randf() * 100,
			0
		},
	};

	obj.attributes.current_health = obj.attributes.max_health;

	const Vector3 hitbox_half_size = Vector3Scale(params.hitbox_size, 0.5f);
	obj.hitbox.box.min = Vector3Subtract(obj.hitbox.position, hitbox_half_size);
	obj.hitbox.box.max = Vector3Add(obj.hitbox.position, hitbox_half_size);

	return obj;
}

void object_move_position(object_t *obj, const Vector3 velocity)
{
	obj->position = Vector3Add(obj->position, velocity);

	obj->hitbox.position = Vector3Add(obj->hitbox.position, velocity);
	const Vector3 hitbox_half_size = Vector3Scale(obj->hitbox.size, 0.5f);
	obj->hitbox.box.min = Vector3Subtract(obj->hitbox.position, hitbox_half_size);
	obj->hitbox.box.max = Vector3Add(obj->hitbox.position, hitbox_half_size);
}

void object_update_position(object_t *obj, const Vector3 position)
{
	Vector3 position_hitbox_offset = Vector3Subtract(obj->position, obj->hitbox.position);
	obj->position = position;

	obj->hitbox.position = Vector3Add(position, position_hitbox_offset);
	const Vector3 hitbox_half_size = Vector3Scale(obj->hitbox.size, 0.5f);
	obj->hitbox.box.min = Vector3Subtract(obj->hitbox.position, hitbox_half_size);
	obj->hitbox.box.max = Vector3Add(obj->hitbox.position, hitbox_half_size);
}
