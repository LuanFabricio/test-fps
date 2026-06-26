#pragma once

#include <stdbool.h>

#include "shapes.h"

bool collision_check_cube(cube_t cube1, cube_t cube2);
bool collision_check_sphere(sphere_t sphere1, sphere_t sphere2) ;
bool collision_check_sphere_cube(sphere_t sphere, cube_t cube);
sphere_t collision_check_sphere_cube_step(const sphere_t sphere_start, const sphere_t sphere_end, cube_t cube, const int steps);
