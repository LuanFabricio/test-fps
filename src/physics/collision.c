#include <raylib.h>
#include <raymath.h>

#include "physics/collision.h"

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
