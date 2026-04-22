#pragma once

#include <raylib.h>

#include "shapes.h"
#include "general.h"

void game_add_sphere(game_t *game, sphere_t sphere, Color color);
void game_add_lines(game_t *game, line_t line, Color color);
void game_add_cubes(game_t *game, cube_t cube, Color color);
