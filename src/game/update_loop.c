#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "camera.h"
#include "dynamic_array.h"
#include "macros/utils.h"
#include "physics/collision.h"
#include "physics/gravity.h"
#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "shapes.h"
#include "ui/ui.h"

typedef enum {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_LEN,
} axis_e;

static void update_ui(game_t *game);
static void update_entities_data(game_t *game, const float delta_time);
static void update_entities_position(game_t *game, const float delta_time);
static void add_entities(entities_t *entities, entities_velocity_t *entities_velocity);
static void entity_update_and_check_collision(cube_t *entity_collision, Vector3 axis_velocity, game_t* game, bool collided_axis[AXIS_LEN]);
static void resolve_axis_collision_cube(cube_t *cube, const cube_t collided, const axis_e axis, const float diff);
static bool check_and_resolve_collision_cube(cube_t* cube, game_t* game, const axis_e axis, const float diff);

inline void game_update_loop(game_t *game, const float delta_time)
{
	update_ui(game);
	if (game->on_pause) return;

	update_entities_data(game, delta_time);
	update_entities_position(game, delta_time);
}

static void update_entities_position(game_t *game, const float delta_time)
{
	const float angle = 10 * delta_time;
	const float radians = angle * DEG2RAD;
	const Vector3 rotate_vec = {0.5f, 0, 0.5f};

	Vector3 *sphere_center = &game->spheres.items[0].center;
	*sphere_center = Vector3RotateByAxisAngle(*sphere_center, rotate_vec, radians);

	cube_t collision;
	Vector3 axis_velocity;
	bool collided_axis[AXIS_LEN] = {false};

	da_for(&game->entities, i) {
		entity_t *entity = &game->entities.items[i];

		collision = entity->hitbox;

		axis_velocity = game->entities_velocity.items[i];
		memset(collided_axis, false, sizeof(bool) * AXIS_LEN);
		entity_update_and_check_collision(&collision, axis_velocity, game, collided_axis);
		entity_update_position(entity, collision.center);
	}

	collision = game->player_collision;
	axis_velocity = Vector3Scale(game->player_velocity, delta_time);
	memset(collided_axis, false, sizeof(bool) * AXIS_LEN);
	entity_update_and_check_collision(&collision, axis_velocity, game, collided_axis);
	if (collided_axis[AXIS_Y]) {
		// NOTE: Maybe move to a function (player_reset_jump)
		game->player_jumps_remaning = game->player_jumps_max;
	}

	game->player_collision.center = collision.center;
	camera_set_position(&game->camera, game->player_collision.center);
}

static void entity_update_and_check_collision(cube_t *entity_collision, const Vector3 axis_velocity, game_t* game, bool collided_axis[AXIS_LEN])
{
	entity_collision->center.x += axis_velocity.x;
	collided_axis[AXIS_X] = check_and_resolve_collision_cube(entity_collision, game, AXIS_X, axis_velocity.x);

	entity_collision->center.y += axis_velocity.y;
	collided_axis[AXIS_Y] = check_and_resolve_collision_cube(entity_collision, game, AXIS_Y, axis_velocity.y);
	if (!collided_axis[AXIS_Y] && collision_check_cube(*entity_collision, game->floor)) {
		resolve_axis_collision_cube(entity_collision, game->floor, AXIS_Y, axis_velocity.y);
		collided_axis[AXIS_Y] = true;
	}

	entity_collision->center.z += axis_velocity.z;
	collided_axis[AXIS_Z] = check_and_resolve_collision_cube(entity_collision, game, AXIS_Z, axis_velocity.z);
}

static void resolve_axis_collision_cube(cube_t *cube, cube_t collided, const axis_e axis, const float diff)
{
	assert(axis < AXIS_LEN);

	if (diff == 0) return;
	switch (axis) {
		case AXIS_X: {
				     const float x_dist = (cube->size.x + collided.size.x) / 2 - collided.center.x;
				     if (diff > 0) {
					     cube->center.x = -x_dist;
				     } else {
					     cube->center.x = x_dist;
				     }
			     } break;
		case AXIS_Y: {
				     const float y_dist = (cube->size.y + collided.size.y) / 2 + collided.center.y;
				     if (diff > 0) {
					     cube->center.y = -y_dist;
				     } else { cube->center.y = y_dist;
				     }
			     } break;
		case AXIS_Z: {
				     const float z_dist = (cube->size.z + collided.size.z) / 2 - collided.center.z;
				     if (diff > 0) {
					     cube->center.z = -z_dist;
				     } else {
					     cube->center.z = z_dist;
				     }
			     } break;
		default:
			     UNREACHABLE("Axis not found");
	}
}

static bool check_and_resolve_collision_cube(cube_t* cube, game_t* game, const axis_e axis, const float diff)
{
	da_for_each(&game->cubes, cube_t) {
		// NOTE: Maybe return the point where collides, and use it to
		// revert the postion with the move direction


		if (collision_check_cube(*cube, *loop.item)) {
			printf("%.4f, %.4f, %.4f (%.4f, %.4f, %.4f/%.4f)\n", cube->center.x, cube->center.y, cube->center.z, cube->size.x, cube->size.y, cube->size.z, diff);
			printf("%p: %.4f, %.4f, %.4f (%.4f, %.4f, %.4f)\n", loop.item, loop.item->center.x, loop.item->center.y, loop.item->center.z, loop.item->size.x, loop.item->size.y, loop.item->size.z);
			resolve_axis_collision_cube(cube, *loop.item, axis, diff);
			return true;
		}
	}
	return false;
}

static void update_ui(game_t *game)
{
	if (IsWindowResized()) {
		printf("Window resized!\n");
		game->screen.width = GetScreenWidth();
		game->screen.height = GetScreenHeight();

		ui_player_info_update_position(game->screen.width, game->screen.height);
	}

	if (game->show_upgrades) {
		ui_player_info_update(&game->player_info);
	}
}

void update_entities_data(game_t *game, const float delta_time)
{
	da_for(&game->rays, i) {
		const ray_t ray = game->rays.items[i];
		if (ray.death_time <= GetTime()) {
			da_remove(&game->rays, i);
			if (i > 0) i--;
		}
	}


	if (game->delay_to_next_shoot > 0) {
		game->delay_to_next_shoot -= delta_time;
	}

	if (game->entities.size <= 5) {
		add_entities(&game->entities, &game->entities_velocity);
	}

	da_for(&game->entities, i) {
		entity_t *entity = &game->entities.items[i];


		if (entity->attributes.current_health <= 0) {
			player_info_gain_xp(
				&game->player_info,
				entity->attributes.max_health + randf() * 100);

			da_remove(&game->entities, i);
			if (i > 0) i--;
		}

		const float speed = 1.5f;
		game->entities_velocity.items[i] = Vector3Scale(
			Vector3Normalize(
				Vector3Subtract(
					game->camera.position, entity->position)),
			speed * delta_time
		);
		gravity_apply(&game->entities_velocity.items[i], delta_time);
	}

	gravity_apply(&game->player_velocity, delta_time);
}

static void add_entities(entities_t *entities, entities_velocity_t *entities_velocity)
{
	const Vector3 position = {
		.x = 30 - randf() * 15,
		.y = 30 - randf() * 15,
		.z = 30 - randf() * 15,
	};
	const Vector3 size = {
		.x = 0.5f + randf() * 2,
		.y = 0.5f + randf() * 2,
		.z = 0.5f + randf() * 2,
	};


	const Color color = {
		.r = randf() * 255,
		.g = randf() * 255,
		.b = randf() * 255,
		.a = 255,
	};

	da_append(
		entities,
		entity_create(
			.position = position,
			.size = size,
			.color = color,
			.attributes = attributes_gen_random(10, 250),
		));
	da_append(entities_velocity, Vector3Zero());
	printf("entities.size = %lu\n", entities->size);
}
