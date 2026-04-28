#include <math.h>
#include <stddef.h>

#include <raylib.h>

#include "game.h"

#define GRAVITY 100.f
#define OPSITE_FORCE 10.f

#define sign_oposite(x) -(signbit((x)) ? -1 : 1)


void gravity_apply(Vector3 *velocity, const float delta_time)
{
	velocity->y = fmaxf(velocity->y - GRAVITY * delta_time, GAME_MIN_VELOCITY_Y);
}
