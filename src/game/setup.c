#include <raymath.h>
#include <time.h>

#include "camera.h"
#include "entity.h"

#include "game.h"
#include "shapes.h"
#include "system/attributes.h"

void game_setup(game_t *game)
{
	srand(time(NULL));

	game->player_info.attributes = attributes_gen_random(10.f, 100.f);
	game->player_info.attributes.damage = 20;

	game->cooldown = 0.166;
	game->delay_to_next_shoot = 0;
	game->screen.width = GetScreenWidth();
	game->screen.height = GetScreenHeight();
	camera_setup(&game->camera);

	game->player_collision = (cube_t){
		.center = { 2, 0, 0, },
		.size = { 0.5, 2.0, 0.5 }
	};
	game->player_jumps_remaning = 2;
	game->player_jumps_max = 2;
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

	entities_data_append(
		&game->entities_data,
		entity_create(
			.position = { 8, 8, 8 },
			.size = { 2, 2, 2 },
			.color = PURPLE,
			.attributes = attributes_gen_random(10, 250),
		),
		Vector3Zero());

	const cube_t floor_cube = {
		.center = {0, -20.f, 0},
		.size = {200.f, 20.f, 200.f},
	};
	game->floor = floor_cube;
}
