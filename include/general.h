#pragma once

#include <stdbool.h>

#include "raylib.h"

#include "shapes.h"
#include "dynamic_array.h"
#include "system/attributes.h"
#include "system/player_info.h"

typedef struct {
	line_t line;
	Ray ray;
	double death_time;
} ray_t;

typedef struct {
	Vector3 position;
	Vector3 size;
	Color color;
	struct {
		BoundingBox box;
		Vector3 position, size;
	} hitbox;
	attributes_t attributes;
} object_t;

typedef struct {
	Camera camera;

	bool show_upgrades;

	Vector2 deg_rotation;
	float cooldown, delay_to_next_shoot;

	struct {
		int width, height;
	} screen;

	player_info_t player;

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

	da_create(object_t) objects;

	bool on_pause;
} game_t;

float randf();
