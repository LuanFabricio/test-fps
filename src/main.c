#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "object.h"
#include "raylib.h"
#include "raymath.h"

#include "camera.h"
#include "dynamic_array.h"
#include "general.h"
#include "input.h"
#include "render/render.h"
#include "system/attributes.h"
#include "ui/ui.h"

void setup(game_t *game)
{
	srand(time(NULL));

	game->player.attributes.damage = 15.f;

	game->cooldown = 0.166;
	game->delay_to_next_shoot = 0;
	game->screen.width = GetScreenWidth();
	game->screen.height = GetScreenHeight();
	camera_setup(&game->camera);

	game->models.cube = LoadModelFromMesh(GenMeshCube(1, 1, 1));

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
		.center = {0}, .size = (Vector3){0.5f, 0.5f, 0.5f},
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
			.size = { 2, 2, 2},
			.hitbox_position = { 8, 8, 8},
			.hitbox_size = { 2, 2, 2},
			.color = PURPLE));
}

static inline void update_loop(game_t *game, const float delta_time)
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

		ui_player_info_update_position(game->screen.width, game->screen.height);
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

	// while (game->player.attribute_points > 0) {
	// 	player_info_spend_skill_upgrade(
	// 		&game->player, rand() % ATTRIBUTE_LEN);
	// }


	if (game->show_upgrades) {
		ui_player_info_update(&game->player);
	}
}

static inline void draw(const game_t game)
{
	BeginDrawing();

	BeginMode3D(game.camera);

	{
		ClearBackground(BLACK);
		for (size_t i = 0; i < game.lines.size; i++) {
			const line_t line = game.lines.items[i];
			DrawLine3D(line.start, line.end, line.color);
		}

		for (size_t i = 0; i < game.rays.size; i++) {
			const ray_t ray = game.rays.items[i];
			DrawLine3D(ray.line.start, ray.line.end, ray.line.color);
		}

		for (size_t i = 0; i < game.cubes.size; i++) {
			const cube_t cube = game.cubes.items[i];
			DrawCubeV(cube.center, cube.size, cube.color);
		}

		for (size_t i = 0; i < game.spheres.size; i++) {
			const sphere_t sphere = game.spheres.items[i];
			DrawSphere(sphere.center, sphere.radius, sphere.color);
		}

		for (size_t i = 0; i < game.objects.size; i++) {
			const object_t object = game.objects.items[i];

			DrawModelEx(
				game.models.cube,
				object.position,
				(Vector3){0, 1, 0},
				0,
				object.size,
				object.color);
			DrawBoundingBox(object.hitbox.box, GREEN);
		}

		DrawModelEx(
			game.models.cube,
			Vector3Add(
				Vector3Add(
					Vector3Add(
						game.camera.position,
						camera_get_forward(&game.camera)),
					Vector3Scale(camera_get_right(&game.camera), 0.75f)),
				Vector3Scale(game.camera.up, -0.5f)
			),
			game.camera.up,
			game.deg_rotation.x + 35.f,
			(Vector3){0.25f, 0.25f, 0.25f},
			GREEN);
	}

	EndMode3D();

	DrawCircle(
		game.screen.width / 2,
		game.screen.height / 2,
		2.5,
		GREEN);

	const int font_size = 24;
	const int x = 16;
	int y = 16;

	DrawFPS(x, y);

	y += font_size;
	DrawText(
		TextFormat(
			"x: %f / y: %f / z: %f",
			game.camera.position.x,
			game.camera.position.y,
			game.camera.position.z),
		x, y, font_size,
		GREEN);

	y += font_size;
	DrawText(
		TextFormat(
			"up: %f, %f, %f",
			game.camera.up.x,
			game.camera.up.y,
			game.camera.up.z),
		x, y, font_size,
		GREEN);

	const Vector3 forward = camera_get_forward(&game.camera);
	y += font_size;
	DrawText(
		TextFormat(
			"forward: %f, %f, %f",
			forward.x,
			forward.y,
			forward.z),
		x, y, font_size,
		GREEN);

	const Vector2 mouse = GetMousePosition();
	y += font_size;
	DrawText(
		TextFormat(
			"mouse: %f, %f",
			mouse.x,
			mouse.y),
		x, y, font_size,
		GREEN);


	render_player_info(&game.player, (Vector2){ game.screen.width - 255, 16 });

	if (game.show_upgrades) {
		ui_player_info_render(&game.player);
		// render_player_info_ui(
		// 	&game.player,
		// 	(Vector2){.x = game.screen.width, .y = game.screen.height });
	}

	EndDrawing();
}

int main(void)
{
	InitWindow(1280, 800, "Test fps");
	game_t game = {0};
	setup(&game);

	ui_player_info_setup(GetScreenWidth(), GetScreenHeight());

	DisableCursor();
	SetTargetFPS(120);
	SetExitKey(KEY_NULL);

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
