#pragma once

#include <stdint.h>

typedef enum {
	ATTRIBUTE_HEALTH = 0,
	ATTRIBUTE_ARMOR,
	ATTRIBUTE_DAMAGE,
	ATTRIBUTE_ELEMENTAL_MULTIPLIER,

	ATTRIBUTE_LEN,
} attributes_e;

typedef struct {
	float
		current_health,
		max_health,
		armor,
		damage,
		elemental_multiplier;
} attributes_t;

#define ATTRIBUTES_BASE_INCREMENT 10.f

void attributes_add(attributes_t* att1, const attributes_t att2);
attributes_t attributes_gen_random(const float min_value, const float max_value);
