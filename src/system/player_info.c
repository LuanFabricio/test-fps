#include "macros/utils.h"

#include "system/player_info.h"

#define NEXT_LEVEL_XP_MULTIPLIER 1.10f

void player_info_gain_xp(player_info_t* player, const float xp)
{
	player->current_xp += xp;
	while (player->current_xp >= player->next_level_xp) {
		player->current_xp = player->next_level_xp - player->current_xp;
		player->next_level_xp *= NEXT_LEVEL_XP_MULTIPLIER;

		player->attribute_points += 1;
		player->total_attribute_points += 1;
	}
}

// NOTE: Maybe delete later
void _attributes_spend_skill_upgrade(attributes_t* increment_attributes, const attributes_e attribute)
{
	switch (attribute) {
		case ATTRIBUTE_HEALTH: {
		       increment_attributes->max_health += ATTRIBUTES_BASE_INCREMENT;
	       } break;

		case ATTRIBUTE_ARMOR: {
		       increment_attributes->armor += ATTRIBUTES_BASE_INCREMENT;
	       } break;

		case ATTRIBUTE_DAMAGE: {
		       increment_attributes->damage += ATTRIBUTES_BASE_INCREMENT;
	       } break;

		case ATTRIBUTE_ELEMENTAL_MULTIPLIER: {
		       increment_attributes->elemental_multiplier += ATTRIBUTES_BASE_INCREMENT;
	       } break;

		default: UNREACHABLE("Invalid attribute");
	}
}
