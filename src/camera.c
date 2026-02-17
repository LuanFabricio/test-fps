#include "raylib.h"
#include "raymath.h"

#include "camera.h"

void camera_setup(Camera* camera)
{
	camera->position = (Vector3){2, 1, 2 };
	camera->fovy = 90;
	camera->projection = CAMERA_PERSPECTIVE;
	camera->target = (Vector3){1, 1 ,1};
	camera->up = (Vector3){0, 1, 0};
}

Vector3 camera_get_forward(const Camera* camera)
{
	return Vector3Subtract(camera->target, camera->position);
}

Vector3 camera_get_right(const Camera* camera)
{
	const Vector3 right = Vector3CrossProduct(
		camera_get_forward(camera),
		camera->up
	);
	return Vector3Normalize(right);
}

void camera_move_forward(Camera* camera, const float speed)
{
	const Vector3 forward = Vector3Scale(camera_get_forward(camera), speed);
	camera->position = Vector3Add(camera->position, forward);
	camera->target = Vector3Add(camera->target, forward);
}
void camera_move_right(Camera* camera, const float speed)
{
	const Vector3 right = Vector3Scale(camera_get_right(camera), speed);
	camera->position = Vector3Add(camera->position, right);
	camera->target = Vector3Add(camera->target, right);
}
void camera_move_up(Camera* camera, const float speed)
{
	const Vector3 up = Vector3Scale(camera->up, speed);
	camera->position = Vector3Add(camera->position, up);
	camera->target = Vector3Add(camera->target, up);
}

void camera_rotate_x(Camera* camera, const float angle)
{
	const Vector3 forward = camera_get_forward(camera);
	camera->target = Vector3Add(
		camera->position,
		Vector3RotateByAxisAngle(forward, camera->up, angle * DEG2RAD)
	);
}
void camera_rotate_y(Camera* camera, const float angle)
{
	const Vector3 forward = camera_get_forward(camera);
	const Vector3 right = camera_get_right(camera);
	camera->target = Vector3Add(
		camera->position,
		Vector3RotateByAxisAngle(forward, right, angle * DEG2RAD)
	);
}
