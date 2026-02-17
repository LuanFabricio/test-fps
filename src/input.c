#include "raymath.h"

#include "camera.h"

#include "input.h"

#define CAMERA_SPEED 1.5f
#define MOUSE_SPEED -5.f

void input_keyboard_handler(Camera *camera, const float delta_time)
{
	Vector3 move_vector = {0};

	// if (IsKeyDown(KEY_LEFT)) {
	// 	move_camera_x(15 * GetFrameTime());
	// }
	// if (IsKeyDown(KEY_RIGHT)) {
	// 	move_camera_x(-15 * GetFrameTime());
	// }

	// if (IsKeyDown(KEY_UP)) {
	// 	move_camera_y(15 * GetFrameTime());
	// }
	// if (IsKeyDown(KEY_DOWN)) {
	// 	move_camera_y(-15 * GetFrameTime());
	// }

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

	const float fixed_speed = CAMERA_SPEED * delta_time;
	move_vector = Vector3Scale(
		Vector3Normalize(move_vector),
		fixed_speed
	);
	camera_move_right(camera, move_vector.x);
	camera_move_up(camera, move_vector.y);
	camera_move_forward(camera, move_vector.z);
}

void input_mouse_handler(Camera *camera, const float delta_time)
{
	const Vector2 mouse_delta = GetMouseDelta();
	const float fixed_speed = MOUSE_SPEED * delta_time;

	camera_rotate_x(camera, mouse_delta.x * fixed_speed);
	camera_rotate_y(camera, mouse_delta.y * fixed_speed);
}
