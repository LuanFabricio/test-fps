#include <stdio.h>

#include "raylib.h"

#include "system/player_info.h"

#define TEXT_BUFFER_SIZE 0xff

void render_player_info(const player_info_t* info, const Vector2 position)
{
	char buffer[TEXT_BUFFER_SIZE];
	const int font_size = 24;

	const int padding = 10;
	int x = position.x + padding;
	int y = position.y + padding;
	int max_width = 0;
	int current_width = 0;

#define update_and_draw(format, ...)\
	do {\
	snprintf(buffer, TEXT_BUFFER_SIZE, format, __VA_ARGS__);\
	current_width = MeasureText(buffer, font_size);\
	if (current_width > max_width) {\
		max_width = current_width;\
	}\
	DrawText(buffer, x, y, font_size, GREEN);\
	y += font_size;\
	} while(0)

	update_and_draw("ATTRIBUTES%s", "");
	update_and_draw("----------%s", "");
	update_and_draw(
		"HP: %.02f/%.02f",
		info->attributes.current_health,
		info->attributes.max_health);
	update_and_draw(
		"ARMR: %.4f",
		info->attributes.armor);
	update_and_draw(
		"DMG: %.4f",
		info->attributes.damage);
	update_and_draw(
		"ELMNT: %.4f%%",
		info->attributes.elemental_multiplier);
	update_and_draw("----------%s", "");

	update_and_draw("XP%s", "");
	update_and_draw("----------%s", "");
	update_and_draw("LVL: %02d", info->level);

	update_and_draw(
		"XP: %.04f/%.04f",
		info->current_xp,
		info->next_level_xp);

	update_and_draw(
		"ATT PTS: %d(%d)",
		info->attribute_points,
		info->total_attribute_points);
#undef update_and_draw

	DrawRectangleLines(
		position.x,
		position.y,
		max_width + padding * 2,
		y - position.y + padding,
		GREEN
	);
}
