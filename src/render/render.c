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

#define format_draw_and_update(format, ...)\
	do {\
		snprintf(buffer, TEXT_BUFFER_SIZE, format, __VA_ARGS__);\
		current_width = MeasureText(buffer, font_size);\
		if (current_width > max_width) {\
			max_width = current_width;\
		}\
		DrawText(buffer, x, y, font_size, GREEN);\
		y += font_size;\
	} while(0)

	format_draw_and_update("ATTRIBUTES%s", "");
	format_draw_and_update("----------%s", "");
	format_draw_and_update(
		"HP: %.02f/%.02f",
		info->attributes.current_health,
		info->attributes.max_health);
	format_draw_and_update(
		"ARMR: %.4f",
		info->attributes.armor);
	format_draw_and_update(
		"DMG: %.4f",
		info->attributes.damage);
	format_draw_and_update(
		"ELMNT: %.4f%%",
		info->attributes.elemental_multiplier);
	format_draw_and_update("----------%s", "");

	format_draw_and_update("XP%s", "");
	format_draw_and_update("----------%s", "");
	format_draw_and_update("LVL: %02d", info->level);

	format_draw_and_update(
		"XP: %.04f/%.04f",
		info->current_xp,
		info->next_level_xp);

	format_draw_and_update(
		"ATT PTS: %d(%d)",
		info->attribute_points,
		info->total_attribute_points);

#undef format_draw_and_update

	DrawRectangleLines(
		position.x,
		position.y,
		max_width + padding * 2,
		y - position.y + padding,
		GREEN
	);
}

void render_player_info_ui(const player_info_t* info, const Vector2 screen_size)
{
	const Color bg_color = BLACK;

	const Vector2 ui_size = { 450, 275 };
	Rectangle bg_rect = {
		.x = (screen_size.x - ui_size.x) / 2.f,
		.y = (screen_size.y - ui_size.y) / 2.f,
		.width = ui_size.x,
		.height = ui_size.y,
	};
	DrawRectangleRec(bg_rect, bg_color);

	int max_width = 0;
	Vector2 current_size;
	int x, y, idx = 0;

	const int font_size = 24;
	const int padding = 16;

	x = (screen_size.x - ui_size.x) / 2.f + padding;
	y = (screen_size.y - ui_size.y) / 2.f + padding;

	struct {
		char text[TEXT_BUFFER_SIZE];
		int x, y, width, height;
	} attribute[4];

	const Font font = GetFontDefault();
	const int font_spacing = font_size / font.baseSize;
#define format_and_update(format, ...)\
	do {\
		snprintf(attribute[idx].text, TEXT_BUFFER_SIZE, format, __VA_ARGS__);\
		current_size = MeasureTextEx(font, attribute[idx].text, font_size, font_spacing);\
		if (current_size.x > max_width) {\
			max_width = current_size.x;\
		}\
		attribute[idx].x = x;\
		attribute[idx].y = y;\
		attribute[idx].width = current_size.x;\
		attribute[idx].height = current_size.y;\
		y += font_size + padding;\
		idx++;\
	} while(0)

	format_and_update(
		"HP: %.02f/%.02f",
		info->attributes.current_health,
		info->attributes.max_health);

	format_and_update(
		"ARMR: %.4f",
		info->attributes.armor);
	format_and_update(
		"DMG: %.4f",
		info->attributes.damage);
	format_and_update(
		"ELMNT: %.4f%%",
		info->attributes.elemental_multiplier);

	const Vector2 mouse_pos = GetMousePosition();
	for (int i = 0; i < idx; i++) {
		DrawText(
			attribute[i].text,
			attribute[i].x,
			attribute[i].y + attribute[i].height / 4.f,
			font_size,
			WHITE);

		Rectangle rect = {
			.x = attribute[i].x + max_width + font_size,
			.y = attribute[i].y,
			.width = 32,
			.height = 32,
		};
		DrawRectangleRec(rect, CheckCollisionPointRec(mouse_pos, rect) ? BLUE : RED);

		DrawRectangleLines(
			attribute[i].x,
			attribute[i].y,
			rect.x - attribute[i].x + 32,
			rect.y - attribute[i].y + 32,
			GREEN
		     );
	}

	// format_and_update("XP%s", "");
	// format_and_update("LVL: %02d", info->level);

	// format_and_update(
	// 	"XP: %.04f/%.04f",
	// 	info->current_xp,
	// 	info->next_level_xp);

	// format_and_update(
	// 	"ATT PTS: %d(%d)",
	// 	info->attribute_points,
	// 	info->total_attribute_points);
}
