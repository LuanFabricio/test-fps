#include <stdio.h>

#include "camera.h"
#include "physics/gravity.h"
#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "object.h"
#include "ui/ui.h"

static void update_ui(game_t *game);
static void update_entities_data(game_t *game, const float delta_time);
static void update_position_by_velocity(Vector3 velocity, Vector3 *position, float delta_time);
static void update_entities_position(game_t *game, const float delta_time);
static void add_objects(objects_t *objects, objects_velocity_t *objects_velocity);

inline void game_update_loop(game_t *game, const float delta_time)
{
	update_ui(game);
	if (game->on_pause) return;

	update_entities_data(game, delta_time);
	update_entities_position(game, delta_time);
}

static void update_position_by_velocity(Vector3 velocity, Vector3 *position, float delta_time)
{
	*position = Vector3Add(*position, Vector3Scale(velocity, delta_time));
}

static void update_entities_position(game_t *game, const float delta_time)
{
	const float angle = 10 * delta_time;
	const float radians = angle * DEG2RAD;
	const Vector3 rotate_vec = {0.5f, 0, 0.5f};

	Vector3 *sphere_center = &game->spheres.items[0].center;
	*sphere_center = Vector3RotateByAxisAngle(*sphere_center, rotate_vec, radians);

	da_for(&game->objects, i) {
		object_t *obj = &game->objects.items[i];
		const Vector3 velocity = Vector3Scale(game->objects_velocity.items[i], delta_time);

		object_move_position(obj, velocity);
	}

	update_position_by_velocity(game->player_velocity, &game->player_position, delta_time);

	camera_set_position(&game->camera, game->player_position);
}

static void update_ui(game_t *game)
{
	if (IsWindowResized()) {
		printf("Window resized!\n");
		game->screen.width = GetScreenWidth();
		game->screen.height = GetScreenHeight();

		ui_player_info_update_position(game->screen.width, game->screen.height);
	}

	if (game->show_upgrades) {
		ui_player_info_update(&game->player_info);
	}
}

void update_entities_data(game_t *game, const float delta_time)
{
	da_for(&game->rays, i) {
		const ray_t ray = game->rays.items[i];
		if (ray.death_time <= GetTime()) {
			da_remove(&game->rays, i);
			if (i > 0) i--;
		}
	}


	if (game->delay_to_next_shoot > 0) {
		game->delay_to_next_shoot -= delta_time;
	}

	if (game->objects.size <= 5) {
		add_objects(&game->objects, &game->objects_velocity);
	}

	da_for(&game->objects, i) {
		object_t *obj = &game->objects.items[i];
		const float speed = 1.5f;
		game->objects_velocity.items[i] = Vector3Scale(
			Vector3Normalize(
				Vector3Subtract(
					game->camera.position, obj->position)),
			speed
		);
	}

	gravity_apply(&game->player_velocity, delta_time);
}

static void add_objects(objects_t *objects, objects_velocity_t *objects_velocity)
{
	const Vector3 position = {
		.x = 30 - randf() * 15,
		.y = 30 - randf() * 15,
		.z = 30 - randf() * 15,
	};
	const Vector3 size = {
		.x = 0.5f + randf() * 2,
		.y = 0.5f + randf() * 2,
		.z = 0.5f + randf() * 2,
	};


	const Color color = {
		.r = randf() * 255,
		.g = randf() * 255,
		.b = randf() * 255,
		.a = 255,
	};

	da_append(
		objects,
		object_create(
			.position = position,
			.size = size,
			.hitbox_position = position,
			.hitbox_size = size,
			.color = color,
		)
	 );
	da_append(objects_velocity, Vector3Zero());
	printf("objects.size = %lu\n", objects->size);
}
