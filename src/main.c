#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "camera.h"
#include "general.h"
#include "input.h"
#include "render/render.h"
#include "shapes.h"
#include "ui/ui.h"

static inline void draw(const game_t game)
{
	// TODO: Check/test a better approach to handle billboards textures
	const size_t entities_attribute_tex_len = game.entities.size;
	Texture *entities_attribute_tex = malloc(sizeof(Texture) * entities_attribute_tex_len);
	BeginDrawing();

	BeginMode3D(game.camera);

	{
		ClearBackground(BLACK);

		DrawCubeV(game.floor.center, game.floor.size, ColorBrightness(WHITE, -0.5f));

		for (size_t i = 0; i < game.lines.size; i++) {
			const line_t line = game.lines.items[i];
			const Color color = game.lines_colors.items[i];
			DrawLine3D(line.start, line.end, color);
		}

		for (size_t i = 0; i < game.rays.size; i++) {
			const ray_t ray = game.rays.items[i];
			const Color color = game.rays_colors.items[i];
			DrawLine3D(ray.line.start, ray.line.end, color);
		}

		for (size_t i = 0; i < game.cubes.size; i++) {
			const cube_t cube = game.cubes.items[i];
			const Color color = game.cubes_colors.items[i];
			DrawCubeV(cube.center, cube.size, color);
		}

		for (size_t i = 0; i < game.spheres.size; i++) {
			const sphere_t sphere = game.spheres.items[i];
			const Color color = game.spheres_colors.items[i];
			DrawSphere(sphere.center, sphere.radius, color);
		}

		for (size_t i = 0; i < game.entities.size; i++) {
			const entity_t entity = game.entities.items[i];

			const Vector3 vec_y = { 0, 1, 0 };
			const float angle = 0;
			DrawModelEx(
				game.models.cube,
				entity.position,
				vec_y,
				angle,
				entity.size,
				entity.color);
			const Vector3 half_size = Vector3Scale(entity.hitbox.size, 0.5f);
			const BoundingBox bounding_box = {
				.min = Vector3Subtract(entity.hitbox.center, half_size),
				.max = Vector3Add(entity.hitbox.center, half_size),
			};
			DrawBoundingBox(bounding_box, GREEN);

			char buffer[0xff] = "";
			const int billboard_font_size = 16;
			snprintf(
				buffer,
				sizeof(buffer),
				"HP: %.02f/%.02f\n"
				"DMG: %.02f\n"
				"ELM MUL: %.02f%%\n",
				entity.attributes.current_health, entity.attributes.max_health,
				entity.attributes.damage,
				entity.attributes.elemental_multiplier);
			Image img = ImageText(buffer, billboard_font_size, WHITE);
			assert(i < entities_attribute_tex_len && "Access out of bounds on `entities_attribute_tex`");
			entities_attribute_tex[i] = LoadTextureFromImage(img);
			Vector3 billboard_position = entity.position;
			billboard_position.y += entity.size.y;
			const float billboard_scale = 1.f;
			DrawBillboard(game.camera, entities_attribute_tex[i], billboard_position, billboard_scale, WHITE);
			UnloadImage(img);
		}

		const Vector3 hand_item_scale = {0.25f, 0.25f, 0.25f};
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
			hand_item_scale,
			GREEN);
	}

	EndMode3D();

	// NOTE: Maybe move to another place
	const float cross_hair_radius = 2.5f;
	DrawCircle(
		game.screen.width / 2,
		game.screen.height / 2,
		cross_hair_radius,
		GREEN);

	// TODO: Move to an module
	const int font_size = 24;
	const int x = 16;
	int y = 16;

	DrawFPS(x, y);
	y += font_size;

	Vector2 info_position = { x, y };
	info_position = ui_draw_camera_info(game.camera, font_size, info_position);
	info_position = ui_draw_player_velocity(game.player_velocity, font_size, info_position);

	render_player_info(&game.player_info, (Vector2){ game.screen.width - 255, 16 });
	if (game.show_upgrades) {
		ui_player_info_render(&game.player_info);
		// render_player_info_ui(
		// 	&game.player,
		// 	(Vector2){.x = game.screen.width, .y = game.screen.height });
	}

	EndDrawing();

	for (size_t i = 0; i < entities_attribute_tex_len; i++) {
		UnloadTexture(entities_attribute_tex[i]);
	}
	free(entities_attribute_tex);
}

int main(void)
{
	InitWindow(1280, 800, "Test fps");
	game_t game = {0};
	game_setup(&game);

	ui_player_info_setup(GetScreenWidth(), GetScreenHeight());

	DisableCursor();
	SetTargetFPS(120);
	SetExitKey(KEY_NULL);

	while(!WindowShouldClose()) {
		draw(game);

		const float frame_time = GetFrameTime();
		game_update_loop(&game, frame_time);

		if (IsWindowFocused()) {
			input_mouse_handler(&game, frame_time);
			input_keyboard_handler(&game, frame_time);
		}
	}

	return 0;
}
