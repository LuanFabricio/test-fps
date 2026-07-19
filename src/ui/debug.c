#include "camera.h"

#include "ui/ui.h"

Vector2 ui_draw_camera_info(const Camera camera, const int font_size, Vector2 position)
{
	DrawText(
		TextFormat(
			"x: %f / y: %f / z: %f",
			camera.position.x,
			camera.position.y,
			camera.position.z),
		position.x, position.y, font_size,
		GREEN);
	position.y += font_size;

	DrawText(
		TextFormat(
			"up: %f, %f, %f",
			camera.up.x,
			camera.up.y,
			camera.up.z),
		position.x, position.y, font_size,
		GREEN);
	position.y += font_size;

	const Vector3 forward = camera_get_forward(&camera);
	DrawText(
		TextFormat(
			"forward: %f, %f, %f",
			forward.x,
			forward.y,
			forward.z),
		position.x, position.y, font_size,
		GREEN);
	position.y += font_size;

	return position;
}

Vector2 ui_draw_player_velocity(const Vector3 velocity, const int font_size, Vector2 position)
{
	DrawText(
		TextFormat(
			"player_velocity: %f, %f, %f",
			velocity.x,
			velocity.y,
			velocity.z),
		position.x, position.y, font_size,
		GREEN);
	position.y += font_size;

	return position;
}
