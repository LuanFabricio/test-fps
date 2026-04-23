#include <time.h>

#include "camera.h"
#include "dynamic_array.h"
#include "object.h"

#include "game.h"

void game_setup(game_t *game)
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


	const line_t x_line = {
		.start = {-xyz_dist, 0, 0},
		.end = {xyz_dist, 0, 0},
	};
	game_add_lines(game, x_line, RED);

	const line_t y_line = {
		.start = {0, -xyz_dist, 0},
		.end = {0, xyz_dist, 0},
	};
	game_add_lines(game, y_line, GREEN);

	const line_t z_line = {
		.start = {0, 0, -xyz_dist},
		.end = {0, 0, xyz_dist},
	};
	game_add_lines(game, z_line, BLUE);

	const cube_t cube1 = {
		.center = {0},
		.size = {0.5f, 0.5f, 0.5f},
	};
	game_add_cubes(game, cube1, YELLOW);

	const sphere_t sphere = {
		.center = {0, 1, 0},
		.radius = PI / 16.f,
	};
	game_add_sphere(
		game,
		sphere,
		PURPLE
	);

	da_append(
		&game->objects,
		object_create(
			.position = { 8, 8, 8},
			.size = { 2, 2, 2},
			.hitbox_position = { 8, 8, 8},
			.hitbox_size = { 2, 2, 2},
			.color = PURPLE));

	const cube_t floor_cube = {
		.center = {0, -5.f, 0},
		.size = {200.f, 5.f, 200.f},
	};
	game->floor = floor_cube;
}

void game_add_sphere(game_t *game, sphere_t sphere, Color color)
{
	da_append(&game->spheres, sphere);
	da_append(&game->spheres_colors, color);
}

void game_add_lines(game_t *game, line_t line, Color color)
{
	da_append(&game->lines, line);
	da_append(&game->lines_colors, color);
}

void game_add_cubes(game_t *game, cube_t cube, Color color)
{
	da_append(&game->cubes, cube);
	da_append(&game->cubes_colors, color);
}
