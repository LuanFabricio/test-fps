#pragma once

#include "raylib.h"

void camera_setup(Camera* camera);

Vector3 camera_get_forward(const Camera* camera);
Vector3 camera_get_right(const Camera* camera);

void camera_move_forward(Camera* camera, const float speed);
void camera_move_right(Camera* camera, const float speed);
void camera_move_up(Camera* camera, const float speed);

void camera_rotate_x(Camera* camera, const float angle);
void camera_rotate_y(Camera* camera, const float angle);
