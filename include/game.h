#pragma once

#include <raylib.h>

#include "dynamic_array.h"
#include "shapes.h"
#include "general.h"
#include "entity.h"
#include "system/player_info.h"

#define GAME_MIN_VELOCITY_Y -150.f

typedef struct {
	Camera camera;

	bool show_upgrades;

	Vector2 deg_rotation;
	float cooldown, delay_to_next_shoot;

	struct {
		int width, height;
	} screen;

	// TODO: Move that struct
	player_info_t player_info;
	int player_jumps_remaning;
	int player_jumps_max;
	cube_t player_collision;
	Vector3 player_velocity;

	struct {
		Model cube;
	} models;

	da_create(line_t) lines;
	da_create(Color) lines_colors;

	da_create(sphere_t) spheres;
	da_create(Color) spheres_colors;

	da_create(ray_t) rays;
	da_create(Color) rays_colors;

	da_create(cube_t) cubes;
	da_create(Color) cubes_colors;

	entities_data_t entities_data;

	bool on_pause;

	cube_t floor;
} game_t;

void game_setup(game_t *game);

void game_update_loop(game_t *game, const float delta_time);

void game_add_sphere(game_t *game, sphere_t sphere, Color color);
void game_add_lines(game_t *game, line_t line, Color color);
void game_add_cubes(game_t *game, cube_t cube, Color color);
