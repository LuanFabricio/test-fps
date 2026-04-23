#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "object.h"
#include "ui/ui.h"

inline void game_update_loop(game_t *game, const float delta_time)
{
	if (IsWindowResized()) {
		printf("Window resized!\n");
		game->screen.width = GetScreenWidth();
		game->screen.height = GetScreenHeight();

		ui_player_info_update_position(game->screen.width, game->screen.height);
	}
	if (game->on_pause) return;

	const float angle = 10 * delta_time;
	const float radians = angle * DEG2RAD;
	const Vector3 rotate_vec = {0.5f, 0, 0.5f};

	Vector3 *sphere_center = &game->spheres.items[0].center;
	*sphere_center = Vector3RotateByAxisAngle(*sphere_center, rotate_vec, radians);

	da_for(&game->rays, i) {
		const ray_t ray = game->rays.items[i];
		if (ray.death_time <= GetTime()) {
			da_remove(&game->rays, i);
			if (i > 0) i--;
		}
	}

	da_for(&game->objects, i) {
		object_t *obj = &game->objects.items[i];
		const float speed = 1.5f;
		const Vector3 move = Vector3Scale(
			Vector3Normalize(
				Vector3Subtract(
					game->camera.position, obj->position)),
			speed * delta_time
		);
		object_move_position(obj, move);
	}

	if (game->delay_to_next_shoot > 0) {
		game->delay_to_next_shoot -= delta_time;
	}

	if (game->objects.size <= 5) {
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
		da_append(
			&game->objects,
			object_create(
				.position = position,
				.size = size,
				.hitbox_position = position,
				.hitbox_size = size,
				.color = (Color){
					.r = randf() * 255,
					.g = randf() * 255,
					.b = randf() * 255,
					.a = 255,
				}
			)
		);
		printf("game->objects.size = %lu\n", game->objects.size);
	}

	if (game->show_upgrades) {
		ui_player_info_update(&game->player);
	}
}
