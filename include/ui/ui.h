#pragma once

#include "raylib.h"

#include "dynamic_array.h"
#include "system/player_info.h"

typedef struct {
	// TODO: Remove this callback, maybe will be better
	// if the module handles the on_over_event as an
	// inner step in the _update function
	void (*on_mouse_over_cb)(void*);
	void (*on_mouse_click_cb)(void*);
	void (*on_render_cb)(void*);
} interactable_t;

typedef struct {
	char content[0xff];
	int x, y, width, height;

	Color color;
	interactable_t interactable;
} text_t;

typedef struct {
	text_t text;
	int x, y, width, height;


	Color bg_base_color;
	Color bg_on_mouse_over_color;

	interactable_t interactable;

	bool selected;

	void* data;
} button_t;

typedef struct {
	Color bg_color;
	Rectangle bg_rec;

	da_create(button_t) buttons;
	da_create(text_t) texts;
} ui_t;


void ui_player_info_setup(const int screen_width, int screen_height);
void ui_player_info_render();
void ui_player_info_update(player_info_t* info);
