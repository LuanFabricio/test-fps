#pragma once

#include "system/attributes.h"

typedef enum {
	ENEMY_DIFICULTY_EASY 	= 1,
	ENEMY_DIFICULTY_MEDIUM 	= 2,
	ENEMY_DIFICULTY_HARD 	= 3
} enemy_dificulty_e;

typedef struct {
	attributes_t attributes;

	uint8_t level;
	enemy_dificulty_e dificulty;
} enemy_info;
