#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "camera.h"
#include "input.h"
#include "raylib.h"
#include "raymath.h"

typedef struct {
	Vector3 start, end;
	Color color;
} line_t;

typedef struct {
	struct {
		line_t *items;
		size_t size, capacity;
	} lines;
} game_t;

const static Vector3 cube_position = (Vector3){0};

const float dist = 0.5f;
static Vector3 sphere_center = (Vector3){0, dist, 0};

#define da_append(arr, x)\
	do {\
		if ((arr)->items == NULL || (arr)->capacity <= 0){\
			free((arr)->items);\
			(arr)->size = 0;\
			(arr)->capacity = 5;\
			(arr)->items = malloc(sizeof(x) * (arr)->capacity);\
		} else if (((arr)->size + 1) >= (arr)->capacity) {\
				(arr)->capacity *= 2;\
				(arr)->items = realloc((arr)->items, sizeof(*(arr)->items) * (arr)->capacity);\
		}\
		(arr)->items[(arr)->size] = x;\
		(arr)->size += 1;\
	} while(0)

void setup(game_t *game)
{
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
}

void update_loop(const float delta_time)
{
	const float angle = 10 * delta_time;
	const float radians = angle * DEG2RAD;
	const Vector3 rotate_vec = {0.5f, 0, 0.5f};

	sphere_center = Vector3RotateByAxisAngle(sphere_center, rotate_vec, radians);
}

int main(void)
{
	Camera camera = {0};
	camera_setup(&camera);
	InitWindow(1280, 800, "Test fps");

	const float xyz_dist = 300;

	DisableCursor();
	SetTargetFPS(120);

	line_t lines[] = {
		{
			.start = {-xyz_dist, 0, 0},
			.end = {xyz_dist, 0, 0},
			.color = RED,
		},
		{
			.start = {0, -xyz_dist, 0},
			.end = {0, xyz_dist, 0},
			.color = GREEN,
		},
		{
			.start = {0, 0, -xyz_dist},
			.end = {0, 0, xyz_dist},
			.color = BLUE,
		}
	};

	game_t game = {0};
	setup(&game);

	while(!WindowShouldClose()) {
		BeginDrawing();

		BeginMode3D(camera);

		ClearBackground(BLACK);
		DrawCube(cube_position, 0.3f, 0.3f, 0.3f, YELLOW);

		DrawSphere(sphere_center, PI / 16, PURPLE);

		for (int i = 0; i < game.lines.size; i++) {
			const line_t line = lines[i];
			DrawLine3D(line.start, line.end, line.color);
		}

		DrawCube((Vector3){0, -10, 0}, 200, 5, 200, ColorBrightness(WHITE, -0.5f));

		EndMode3D();

		EndDrawing();

		const float delta_time = GetFrameTime();

		update_loop(delta_time);
		if (IsWindowFocused()) {
			input_mouse_handler(&camera, delta_time);
			input_keyboard_handler(&camera, delta_time);
		}
	}

	return 0;
}
