#pragma once

#include <stdbool.h>

#include "shapes.h"

bool collision_check_sphere(sphere_t sphere1, sphere_t sphere2) ;
bool collision_check_sphere_cube(sphere_t sphere, cube_t cube);
