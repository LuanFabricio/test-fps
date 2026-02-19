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

	if (IsKeyUp(KEY_SPACE)) {
		move_vector.y += -1;
	}
	if (IsKeyUp(KEY_LEFT_SHIFT)) {
		move_vector.y += 1;
	}

	return Vector3Normalize(move_vector);
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
	camera_rotate_x(camera, mouse_delta.x * fixed_speed);
	camera_rotate_y(camera, mouse_delta.y * fixed_speed);

	const bool mouse_left_click = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	if (mouse_left_click) {
		const Vector2 aim_position = GetMousePosition();
		// {
		// 	.x = game->screen.width / 2.f,
		// 	.y = game->screen.height / 2.f,
		// };

		Ray ray = GetScreenToWorldRay(aim_position, game->camera);
		da_append(&game->rays, ((ray_t){
			.line = {
				.color = WHITE,
				.start = ray.position,
				.end = Vector3Add(ray.position, Vector3Scale(ray.direction, 300)),
			},
			.ray = ray,
			.death_time = GetTime() + 10,
		}));
	}
}
