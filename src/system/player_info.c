#include "macros/utils.h"

#include "system/player_info.h"

#define BASE_XP_INCREASE 10.f
#define NEXT_LEVEL_XP_MULTIPLIER 1.10f

void player_info_gain_xp(player_info_t* player, const float xp)
{
	player->current_xp += xp;
	while (player->current_xp >= player->next_level_xp) {
		player->current_xp = player->next_level_xp - player->current_xp;

		player->next_level_xp += BASE_XP_INCREASE;
		player->next_level_xp *= NEXT_LEVEL_XP_MULTIPLIER;

		player->attribute_points += 1;
		player->total_attribute_points += 1;
		player->level += 1;
	}

	if (player->current_xp < 0) {
		player->current_xp = 0.f;
	}
}

// NOTE: Maybe delete later
void player_info_spend_skill_upgrade(player_info_t* info, const attributes_e attribute)
{
	if (info->attribute_points <= 0) return;

	switch (attribute) {
		case ATTRIBUTE_HEALTH: {
		       info->attributes.max_health += ATTRIBUTES_BASE_INCREMENT;
		       info->attributes.current_health += ATTRIBUTES_BASE_INCREMENT;
	       } break;

		case ATTRIBUTE_ARMOR: {
		       info->attributes.armor += ATTRIBUTES_BASE_INCREMENT;
	       } break;

		case ATTRIBUTE_DAMAGE: {
		       info->attributes.damage += ATTRIBUTES_BASE_INCREMENT;
	       } break;

		case ATTRIBUTE_ELEMENTAL_MULTIPLIER: {
		       info->attributes.elemental_multiplier += ATTRIBUTES_BASE_INCREMENT;
	       } break;

		default: UNREACHABLE("Invalid attribute");
	}

	info->attribute_points -= 1;
}
