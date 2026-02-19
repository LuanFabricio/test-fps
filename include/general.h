#pragma once

#include "raylib.h"

#include "dynamic_array.h"

typedef struct {
	Vector3 start, end;
	Color color;
} line_t;

typedef struct {
	Vector3 center;
	float radius;
	Color color;
} sphere_t;

typedef struct {
	Vector3 center, size;
	Color color;
} cube_t;

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
} object_t;

typedef struct {
	Camera camera;

	struct {
		int width, height;
	} screen;

	da_create(line_t) lines;
	da_create(sphere_t) spheres;
	da_create(cube_t) cubes;
	da_create(ray_t) rays;
	da_create(object_t) objects;
} game_t;
