#pragma once

#include "attributes.h"

typedef struct {
	attributes_t attributes;

	float current_xp,
		next_level_xp;

	int32_t level,
		attribute_points,
		total_attribute_points;
} player_info_t;

void player_info_gain_xp(player_info_t* entity, const float xp);
void player_info_spend_skill_upgrade(player_info_t* info, const attributes_e attribute);
