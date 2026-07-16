#include <stdbool.h>
#include <stdio.h>

#include "entity.h"
#include "macros/utils.h"
#include "raylib.h"
#include "raymath.h"

#include "camera.h"
#include "game.h"
#include "general.h"

#include "input.h"

#define CAMERA_SPEED 2.5f
#define CAMERA_SPRINT_MULTIPLIER 4.5f
#define MOUSE_SPEED -5.f

#define GAME_KEY_INVENTORY KEY_TAB
#define GAME_KEY_PAUSE KEY_P

static bool last_frame_resized = true;

inline static Vector3 get_move_vector(const Camera* camera)
{
	Vector3 move_vector = {0};
	const Vector3 right = camera_get_right(camera);
	if (IsKeyDown(KEY_A)) {
		move_vector = Vector3Subtract(move_vector, right);
	}
	if (IsKeyDown(KEY_D)) {
		move_vector = Vector3Add(move_vector, right);
	}

	const Vector3 forward = camera_get_forward(camera);
	if (IsKeyDown(KEY_W)) {
		move_vector = Vector3Add(move_vector, forward);
	}
	if (IsKeyDown(KEY_S)) {
		move_vector = Vector3Subtract(move_vector, forward);
	}

	move_vector = Vector3Normalize(move_vector);
	// NOTE: With gravity this is useless
	move_vector.y = 0;

	if (IsKeyPressed(KEY_SPACE)) {
		move_vector.y = 45;
	}
	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		move_vector.y += -1;
	}

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		move_vector.x = move_vector.x * CAMERA_SPRINT_MULTIPLIER;
		move_vector.z = move_vector.z * CAMERA_SPRINT_MULTIPLIER;
	}

	return move_vector;
}

void input_keyboard_handler(game_t *game, const float delta_time)
{
	Camera *camera = &game->camera;

	Vector3 new_velocity = get_move_vector(camera);
	// TODO: Improve this, to structure the gravity + jumps
	if (game->player_jumps_remaning <= 0) {
		new_velocity.y = 0;
	} else if (new_velocity.y > 0) {
		game->player_jumps_remaning--;
	}
	if (new_velocity.y == 0) {
		new_velocity.y = game->player_velocity.y;
	}
	game->player_velocity = new_velocity;
	UNUSED(delta_time);

	// camera_set_position(camera, Vector3Add(camera->position, move_vector));

	// camera_move_right(camera, move_vector.x);
	// camera_move_up(camera, move_vector.y);
	// camera_move_forward(camera, move_vector.z);

	if (IsKeyPressed(GAME_KEY_INVENTORY)) {
		game->show_upgrades = !game->show_upgrades;
		if (IsCursorHidden()) {
			EnableCursor();
		} else {
			DisableCursor();
		}
	} else if (IsKeyPressed(GAME_KEY_PAUSE)) {
		game->on_pause = !game->on_pause;
	}
}

void input_mouse_handler(game_t *game, const float delta_time)
{
	if (game->show_upgrades) return;
	if (last_frame_resized) {
		last_frame_resized = false;
		return;
	}
	last_frame_resized = IsWindowResized();

	const Vector2 mouse_delta = GetMouseDelta();
	const float fixed_speed = MOUSE_SPEED * delta_time;

	Camera *camera = &game->camera;
	const float angle_x = mouse_delta.x * fixed_speed;
	const float angle_y = mouse_delta.y * fixed_speed;

	game->deg_rotation.x += angle_x;
	game->deg_rotation.y += angle_y;
	camera_rotate_x(camera, angle_x);
	camera_rotate_y(camera, angle_y);

	const bool mouse_left_click = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	if (mouse_left_click && game->delay_to_next_shoot <= 0) {
		game->delay_to_next_shoot = game->cooldown;
		const Vector3 camera_forward = camera_get_forward(&game->camera);

		Ray ray = {
			.position = game->camera.position,
			.direction = camera_forward,
		};

		// TODO: Move to a specific module
		da_for(&game->entities, i) {
			entity_t *entity = &game->entities.items[i];
			const Vector3 half_size = Vector3Scale(entity->hitbox.size, 0.5f);
			const BoundingBox bounding_box = {
				.min = Vector3Subtract(entity->hitbox.center, half_size),
				.max = Vector3Add(entity->hitbox.center, half_size),
			};
			RayCollision ray_col = GetRayCollisionBox(ray, bounding_box);

			if (ray_col.hit) {
				entity->attributes.current_health -= game->player_info.attributes.damage;
			}

			// NOTE: This check should be in another loop,
			// maybe a `check_loop` function in `src/game/game_loop.c`
			if (entity->attributes.current_health <= 0) {
				player_info_gain_xp(
					&game->player_info,
					entity->attributes.max_health + randf() * 100);

				da_remove(&game->entities, i);
				if (i > 0) i--;
			}
		}
	}
}
