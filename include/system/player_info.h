#include "attributes.h"

typedef struct {
	attributes_t attributes;

	float
		level,
		current_xp,
		next_level_xp;

	int32_t attribute_points, total_attribute_points;
} player_info_t;

void player_info_gain_xp(player_info_t* entity, const float xp);
void attributes_spend_skill_upgrade(attributes_t* increment_attributes, const attributes_e attribute);
