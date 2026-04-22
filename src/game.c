#include "dynamic_array.h"

#include "game.h"

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
