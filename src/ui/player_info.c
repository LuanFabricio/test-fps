#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <raylib.h>
#include <sys/select.h>

#include "macros/utils.h"
#include "dynamic_array.h"
#include "system/attributes.h"
#include "system/player_info.h"
#include "ui/ui.h"

#define MAX_VALUE_FORMAT 999999999.f
#define TEXT_BUFFER_SIZE 0xff
#define FONT_SIZE 24

struct player_upgrade_bundle {
	player_info_t* info;
	attributes_e attribute;
};

static void on_mouse_over_cb(void* ptr);
static void on_mouse_click_cb(void* ptr);
static void attribute_text_format(const attributes_t* attr, const char* format, attributes_e attribute, char buffer[TEXT_BUFFER_SIZE]);

static ui_t player_ui =  {
	.bg_color = GREEN,
	.bg_rec = {0},
	.buttons = {0},
	.texts = {0},
};

static Font font;

#define append_format_text(text, attr)\
	do {\
		current_size = MeasureTextEx(font, text, FONT_SIZE, font_spacing);\
		da_append(&player_ui.texts, ((text_t){\
			.x = x,\
			.y = y,\
			.width = current_size.x,\
			.height = current_size.y,\
			.color = BLACK,\
		}));\
		*(attributes_e*)player_ui.texts.items[player_ui.texts.size-1].data = attr;\
		memcpy(player_ui.texts.items[player_ui.texts.size-1].content, text, strlen(text));\
		y += current_size.y + padding;\
		idx++;\
	} while(0)

void ui_player_info_setup(const int screen_width, int screen_height)
{
	font = GetFontDefault();

	const int font_spacing = FONT_SIZE / font.baseSize;
	const int padding = 8;

	const int initial_x = screen_width  / 2.f + padding;
	const int initial_y = screen_height / 2.f + padding;


	const int items_to_show = 4;
	char buffer[TEXT_BUFFER_SIZE];
	// Text max width + padding + button width + padding
	snprintf(buffer, TEXT_BUFFER_SIZE, "HP: %.02f/%.02f", MAX_VALUE_FORMAT, MAX_VALUE_FORMAT);
	const Vector2 base_size = MeasureTextEx(font, buffer, FONT_SIZE, font_spacing);
	const int btn_size = base_size.y;
	const int max_width = base_size.x + btn_size + 2 * padding;
	const int max_height = (base_size.y + padding) * items_to_show + padding;
	const int rect_width = max_width + padding;
	const int rect_height = max_height + padding;

	Vector2 current_size;
	int idx = 0;
	int x, y;

	x = initial_x - rect_width / 2.f;
	y = initial_y - rect_height / 2.f;

	// TODO: Create an internal function to append a
	// template and a button, passing out the attributes_e
	append_format_text("HP: %.02f/%.02f", ATTRIBUTE_HEALTH);
	append_format_text("ARMR: %.4f", ATTRIBUTE_ARMOR);
	append_format_text("DMG: %.4f", ATTRIBUTE_DAMAGE);
	append_format_text("ELMNT: %.4f%%", ATTRIBUTE_ELEMENTAL_MULTIPLIER);

	da_for_each(&player_ui.texts, text_t) {
		da_append(&player_ui.buttons, ((button_t){
			.text = (text_t){
				.content = "+",
				.color = WHITE,
			},
			.x = loop.item->x + max_width - btn_size - padding,
			.y = loop.item->y,
			.width = loop.item->height,
			.height = loop.item->height,
			.bg_base_color = BLUE,
			.bg_on_mouse_over_color = RED,
			.selected = false,
			.interactable = {
				.on_mouse_over_cb = &on_mouse_over_cb,
				.on_mouse_click_cb = &on_mouse_click_cb,
			},
			.data = malloc(sizeof(attributes_e)),
		}));

		button_t *last_btn = &player_ui.buttons.items[loop.i];
		*(int*)last_btn->data = loop.i;
		last_btn->text.x = last_btn->x + last_btn->width  / 4;
		last_btn->text.y = last_btn->y;
	}

	player_ui.bg_rec = (Rectangle) {
		.x = (initial_x - padding) - max_width  / 2.f,
		.y = (initial_y - padding) - max_height / 2.f,
		.width = rect_width,
		.height = max_height,
	};
}

void ui_player_info_render(const player_info_t* info)
{
	DrawRectangleRec(player_ui.bg_rec, player_ui.bg_color);

	char buffer[TEXT_BUFFER_SIZE];

	da_for_each(&player_ui.texts, text_t) {
		attribute_text_format(
			&info->attributes,
			loop.item->content,
			*(attributes_e*)loop.item->data,
			buffer);
		DrawText(
			buffer,
			loop.item->x,
			loop.item->y,
			FONT_SIZE,
			loop.item->color);

		DrawRectangleLines(
			loop.item->x,
			loop.item->y,
			player_ui.buttons.items[loop.i].x - loop.item->x,
			loop.item->height,
			WHITE);
	}

	da_for_each(&player_ui.buttons, button_t) {
		Color color = loop.item->bg_base_color;
		if (loop.item->selected) {
			color = loop.item->bg_on_mouse_over_color;
		}
		DrawRectangle(
			loop.item->x,
			loop.item->y,
			loop.item->width,
			loop.item->height,
			color);

		DrawText(
			loop.item->text.content,
			loop.item->text.x,
			loop.item->text.y,
			FONT_SIZE,
			loop.item->text.color);
	}
}

void ui_player_info_update(player_info_t* info)
{
	const Vector2 mouse_position = GetMousePosition();
	const bool is_clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

	da_for_each(&player_ui.buttons, button_t) {
		Rectangle box = {
			.x = loop.item->x,
			.y = loop.item->y,
			.width = loop.item->width,
			.height = loop.item->height,
		};


		const bool mouse_collide = CheckCollisionPointRec(mouse_position, box);
		if (mouse_collide) {
			if (loop.item->interactable.on_mouse_over_cb) {
				loop.item->interactable.on_mouse_over_cb(loop.item);
			}

			if (is_clicked && loop.item->interactable.on_mouse_click_cb) {
				struct player_upgrade_bundle bundle = {
					.info = info,
					.attribute = *(attributes_e*)loop.item->data,
				};
				loop.item->interactable.on_mouse_click_cb(&bundle);
			}
		} else {
			loop.item->selected = false;
		}
	}
}

// TODO: Implement an ui_player_info_update_position
// function and move the UI item placement to it

static void on_mouse_over_cb(void* ptr)
{
	button_t *btn = (button_t*) ptr;
	btn->selected = true;
}

static void on_mouse_click_cb(void* ptr)
{
	struct player_upgrade_bundle* bundle = (struct player_upgrade_bundle*)ptr;
	player_info_spend_skill_upgrade(bundle->info, bundle->attribute);
}


static void attribute_text_format(
	const attributes_t* attr,
	const char* format,
	const attributes_e attribute,
	char buffer[TEXT_BUFFER_SIZE])
{

	switch (attribute) {
		case ATTRIBUTE_HEALTH:
			snprintf(
				buffer, TEXT_BUFFER_SIZE,
				format, attr->current_health, attr->max_health);
			break;
		case ATTRIBUTE_ARMOR:
			snprintf(
				buffer, TEXT_BUFFER_SIZE,
				format, attr->armor);
			break;
		case ATTRIBUTE_DAMAGE:
			snprintf(
				buffer, TEXT_BUFFER_SIZE,
				format, attr->damage);
			break;
		case ATTRIBUTE_ELEMENTAL_MULTIPLIER:
			snprintf(
				buffer, TEXT_BUFFER_SIZE,
				format, attr->elemental_multiplier);
			break;
		default: UNREACHABLE("Attribute not mapped");
	}
}
