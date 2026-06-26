#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "shapes.h"

#include "physics/collision.h"

bool collision_check_cube(cube_t cube1, cube_t cube2)
{
	const Vector3 half_cube1 = Vector3Scale(cube1.size, .5f);
	const float x_min1 = cube1.center.x - half_cube1.x;
	const float y_min1 = cube1.center.y - half_cube1.y;
	const float z_min1 = cube1.center.z - half_cube1.z;

	const float x_max1 = cube1.center.x + half_cube1.x;
	const float y_max1 = cube1.center.y + half_cube1.y;
	const float z_max1 = cube1.center.z + half_cube1.z;

	const Vector3 half_cube2 = Vector3Scale(cube2.size, .5f);
	const float x_min2 = cube2.center.x - half_cube2.x;
	const float y_min2 = cube2.center.y - half_cube2.y;
	const float z_min2 = cube2.center.z - half_cube2.z;

	const float x_max2 = cube2.center.x + half_cube2.x;
	const float y_max2 = cube2.center.y + half_cube2.y;
	const float z_max2 = cube2.center.z + half_cube2.z;

	return (x_min1 < x_max2 && x_min2 < x_max1)
		&& (y_min1 < y_max2 && y_min2 < y_max1)
		&& (z_min1 < z_max2 && z_min2 < z_max1);
}

bool collision_check_sphere(sphere_t sphere1, sphere_t sphere2)
{
	return CheckCollisionSpheres(
		sphere1.center,
		sphere1.radius,
		sphere2.center,
		sphere2.radius
	);
}

bool collision_check_sphere_cube(sphere_t sphere, cube_t cube)
{
	BoundingBox box = {
		.min = Vector3Subtract(cube.center, cube.size),
		.max = Vector3Add(cube.center, cube.size),
	};

	return CheckCollisionBoxSphere(box, sphere.center, sphere.radius);
}

sphere_t collision_check_sphere_cube_step(const sphere_t sphere_start, const sphere_t sphere_end, cube_t cube, const int steps)
{
	sphere_t current_step = sphere_start;
	const Vector3 move_step = Vector3Scale(
		Vector3Subtract(sphere_end.center, sphere_start.center),
		1.f / steps
	);

	for (int step = 0; step < steps; step++) {
		printf("Step %i\n", step);
		printf("\tcurrent_step.center: %f, %f, %f\n", current_step.center.x, current_step.center.y, current_step.center.z);
		printf("\tcurrent_step.radius: %f\n", current_step.radius);
		printf("\tMove step: %f, %f, %f\n", move_step.x, move_step.y, move_step.z);

		sphere_t future_step = current_step;
		future_step.center.x +=  move_step.x;
		if (collision_check_sphere_cube(future_step, cube)) {
			current_step.center.x += move_step.x;
		}

		future_step = current_step;
		future_step.center.y +=  move_step.y;
		if (collision_check_sphere_cube(future_step, cube)) {
			current_step.center.y += move_step.y;
		}

		future_step = current_step;
		future_step.center.z +=  move_step.z;
		if (collision_check_sphere_cube(future_step, cube)) {
			current_step.center.z += move_step.z;
		}
	}

	return current_step;
}
