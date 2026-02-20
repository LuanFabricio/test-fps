#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "object.h"
#include "raylib.h"
#include "raymath.h"

#include "camera.h"
#include "dynamic_array.h"
#include "general.h"
#include "input.h"

void setup(game_t *game)
{
	srand(time(NULL));

	game->screen.width = GetScreenWidth();
	game->screen.height = GetScreenHeight();
	camera_setup(&game->camera);

	const float xyz_dist = 300;
	da_append(&game->lines, ((line_t){
		.start = {-xyz_dist, 0, 0},
		.end = {xyz_dist, 0, 0},
		.color = RED,
	}));
	da_append(&game->lines, ((line_t){
		.start = {0, -xyz_dist, 0},
		.end = {0, xyz_dist, 0},
		.color = GREEN,
	}));
	da_append(&game->lines, ((line_t){
		.start = {0, 0, -xyz_dist},
		.end = {0, 0, xyz_dist},
		.color = BLUE,
	}));

	da_append(&game->cubes, ((cube_t){
		.center = {0},
		.size = (Vector3){0.5f, 0.5f, 0.5f},
		.color = YELLOW,
	}));
	da_append(&game->cubes, ((cube_t){
		.center = {0, -10, 0},
		.size = (Vector3){200.f, 5.f, 200.f},
		.color = ColorBrightness(WHITE, -0.5f),
	}));

	da_append(&game->spheres, ((sphere_t){
		.center = {0, 1, 0},
		.radius = PI / 16.f,
		.color = PURPLE,
	}));

	da_append(
		&game->objects,
		object_create(
			.position = { 8, 8, 8},
			.size = { 8, 8, 8},
			.hitbox_position = { 8, 8, 8},
			.hitbox_size = { 8, 8, 8},
			.color = PURPLE));
}
void update_loop(game_t *game, const float delta_time)
{
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
		da_for(&game->rays, j) {
			Ray ray = game->rays.items[j].ray;
			RayCollision ray_col = GetRayCollisionBox(ray, obj->hitbox.box);

			if (ray_col.hit) {
				game->rays.items[j].line.color = RED;
			}
		}

		const float speed = 1.5f;
		const Vector3 move = Vector3Scale(
			Vector3Normalize(
				Vector3Subtract(
					game->camera.position, obj->position)),
			speed * delta_time
		);
		object_move_position(obj, move);
	}

	if (IsWindowResized()) {
		game->screen.width = GetScreenWidth();
		game->screen.height = GetScreenHeight();
	}
}

void draw(const game_t game)
{
	BeginDrawing();

	BeginMode3D(game.camera);

	{
		ClearBackground(BLACK);
		for (int i = 0; i < game.lines.size; i++) {
			const line_t line = game.lines.items[i];
			DrawLine3D(line.start, line.end, line.color);
		}

		for (int i = 0; i < game.rays.size; i++) {
			const ray_t ray = game.rays.items[i];
			DrawLine3D(ray.line.start, ray.line.end, ray.line.color);
		}

		for (int i = 0; i < game.cubes.size; i++) {
			const cube_t cube = game.cubes.items[i];
			DrawCubeV(cube.center, cube.size, cube.color);
		}

		for (int i = 0; i < game.spheres.size; i++) {
			const sphere_t sphere = game.spheres.items[i];
			DrawSphere(sphere.center, sphere.radius, sphere.color);
		}

		for (int i = 0; i < game.objects.size; i++) {
			const object_t object = game.objects.items[i];
			DrawCubeV(object.position, object.size, object.color);
			DrawCubeWiresV(object.hitbox.position, object.hitbox.size, GREEN);
		}

	}

	EndMode3D();

	DrawText(
		TextFormat(
			"x: %f / y: %f / z: %f",
			game.camera.position.x,
			game.camera.position.y,
			game.camera.position.z),
		32, 32, 32,
		GREEN);
	DrawText(
		TextFormat(
			"up: %f, %f, %f",
			game.camera.up.x,
			game.camera.up.y,
			game.camera.up.z),
		32, 64, 32,
		GREEN);
	const Vector3 forward = camera_get_forward(&game.camera);
	DrawText(
		TextFormat(
			"forward: %f, %f, %f",
			forward.x,
			forward.y,
			forward.z),
		32, 96, 32,
		GREEN);

	const Vector2 mouse = GetMousePosition();
	DrawText(
		TextFormat(
			"mouse: %f, %f",
			mouse.x,
			mouse.y),
		32, 128, 32,
		GREEN);

	EndDrawing();
}

int main(void)
{
	game_t game = {0};
	setup(&game);

	InitWindow(1280, 800, "Test fps");

	const float xyz_dist = 300;

	DisableCursor();
	SetTargetFPS(120);

	while(!WindowShouldClose()) {
		draw(game);

		const float delta_time = GetFrameTime();

		update_loop(&game, delta_time);
		if (IsWindowFocused()) {
			input_mouse_handler(&game, delta_time);
			input_keyboard_handler(&game, delta_time);
		}
	}

	return 0;
}
