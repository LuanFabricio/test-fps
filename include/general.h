#pragma once

#include <stdlib.h>

#include "raylib.h"

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

#define da_create(type)\
	struct {\
		type *items;\
		size_t size, capacity;\
	}

typedef struct {
	da_create(line_t) lines;
	da_create(sphere_t) spheres;
	da_create(cube_t) cubes;
} game_t;

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

#define da_for_each(arr, type)\
	for (struct { size_t i; type* item; } loop = { 0, &(arr)->items[0] } ; loop.i < (arr)->size; loop.i++, loop.item = &(arr)->items[loop.i])
