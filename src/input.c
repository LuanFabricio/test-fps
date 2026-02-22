#include "general.h"
#include "raymath.h"

#include "camera.h"
#include <raylib.h>

#include "input.h"

#define CAMERA_SPEED 1.5f
#define MOUSE_SPEED -5.f

inline static Vector3 get_move_vector()
{
	Vector3 move_vector = {0};

	if (IsKeyDown(KEY_A)) {
		move_vector.x += -1;
	}
	if (IsKeyDown(KEY_D)) {
		move_vector.x += 1;
	}

	if (IsKeyDown(KEY_W)) {
		move_vector.z += 1;
	}
	if (IsKeyDown(KEY_S)) {
		move_vector.z += -1;
	}

	if (IsKeyDown(KEY_SPACE)) {
		move_vector.y += 1;
	}
	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		move_vector.y += -1;
	}

	move_vector = Vector3Normalize(move_vector);

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		move_vector = Vector3Scale(move_vector, 4.5f);
	}

	return move_vector;
}

void input_keyboard_handler(game_t *game, const float delta_time)
{
	Vector3 move_vector = get_move_vector();
	const float fixed_speed = CAMERA_SPEED * delta_time;
	move_vector = Vector3Scale(move_vector, fixed_speed);

	Camera *camera = &game->camera;
	camera_move_right(camera, move_vector.x);
	camera_move_up(camera, move_vector.y);
	camera_move_forward(camera, move_vector.z);
}

void input_mouse_handler(game_t *game, const float delta_time)
{
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
		da_append(&game->rays, ((ray_t){
			.line = {
				.color = WHITE,
				.start = game->camera.position,
				.end = Vector3Add(game->camera.position, Vector3Scale(camera_forward, 300)),
			},
			.ray = {
				.position = game->camera.position,
				.direction = camera_forward,
			},
			.death_time = GetTime() + 10,
		}));
	}
}
