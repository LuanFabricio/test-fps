#include <assert.h>
#include <stdio.h>

#include "camera.h"
#include "dynamic_array.h"
#include "physics/collision.h"
#include "physics/gravity.h"
#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "object.h"
#include "shapes.h"
#include "ui/ui.h"

typedef enum {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
} axis_e;

static void update_ui(game_t *game);
static void update_entities_data(game_t *game, const float delta_time);
static void update_position_by_velocity(Vector3 velocity, Vector3 *position, float delta_time);
static void update_entities_position(game_t *game, const float delta_time);
static void add_objects(objects_t *objects, objects_velocity_t *objects_velocity);
static void resolve_axis_collision_cube(cube_t *cube, cube_t collided, const axis_e axis, const float diff);
static void check_and_resolve_collision_cube(cube_t* cube, game_t* game, const axis_e axis, const float diff);

inline void game_update_loop(game_t *game, const float delta_time)
{
	update_ui(game);
	if (game->on_pause) return;

	update_entities_data(game, delta_time);
	update_entities_position(game, delta_time);
}

static void update_position_by_velocity(Vector3 velocity, Vector3 *position, float delta_time)
{
	*position = Vector3Add(*position, Vector3Scale(velocity, delta_time));
}

static void update_entities_position(game_t *game, const float delta_time)
{
	const float angle = 10 * delta_time;
	const float radians = angle * DEG2RAD;
	const Vector3 rotate_vec = {0.5f, 0, 0.5f};

	Vector3 *sphere_center = &game->spheres.items[0].center;
	*sphere_center = Vector3RotateByAxisAngle(*sphere_center, rotate_vec, radians);

	da_for(&game->objects, i) {
		object_t *obj = &game->objects.items[i];
		const Vector3 velocity = Vector3Scale(game->objects_velocity.items[i], delta_time);

		object_move_position(obj, velocity);
	}

	const float radius = .1f;
	const sphere_t start = {
		.center = game->player_collision.center,
		.radius = radius,
	};
	sphere_t end = {
		.center = game->player_collision.center,
		.radius = radius,
	};
	update_position_by_velocity(game->player_velocity, &end.center, delta_time);
	const Vector3 diff = Vector3Subtract(start.center, end.center);

	da_for_each(&game->objects, object_t) {
		const cube_t bounding_box = {
			.center = loop.item->hitbox.position,
			.size = loop.item->hitbox.size,
		};

		// assert(!collision_check_sphere_cube_step(start, end, bounding_box, 10));
	}

	cube_t player_collision = game->player_collision;

	float speed = game->player_velocity.x * delta_time;
	player_collision.center.x += speed;
	check_and_resolve_collision_cube(&player_collision, game, AXIS_X, speed);

	speed = game->player_velocity.y * delta_time;
	player_collision.center.y += speed;
	check_and_resolve_collision_cube(&player_collision, game, AXIS_Y, speed);
	if (collision_check_cube(player_collision, game->floor)) {
		resolve_axis_collision_cube(&player_collision, game->floor, AXIS_Y, speed);
	}

	speed = game->player_velocity.z * delta_time;
	player_collision.center.z += speed;
	check_and_resolve_collision_cube(&player_collision, game, AXIS_Z, speed);

	game->player_collision.center = player_collision.center;
	camera_set_position(&game->camera, game->player_collision.center);
}

static void resolve_axis_collision_cube(cube_t *cube, cube_t collided, const axis_e axis, const float diff)
{
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
				     } else {
					     cube->center.y = y_dist;
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
			     assert(false && "UNREACHEABLE");
	}
}

static void check_and_resolve_collision_cube(cube_t* cube, game_t* game, const axis_e axis, const float diff)
{
	da_for_each(&game->cubes, cube_t) {
		// NOTE: Maybe return the point where collides, and use it to
		// revert the postion with the move direction


		if (collision_check_cube(*cube, *loop.item)) {
			printf("%.4f, %.4f, %.4f (%.4f, %.4f, %.4f/%.4f)\n", cube->center.x, cube->center.y, cube->center.z, cube->size.x, cube->size.y, cube->size.z, diff);
			printf("%p: %.4f, %.4f, %.4f (%.4f, %.4f, %.4f)\n", loop.item, loop.item->center.x, loop.item->center.y, loop.item->center.z, loop.item->size.x, loop.item->size.y, loop.item->size.z);
			resolve_axis_collision_cube(cube, *loop.item, axis, diff);
			break;
		}
	}
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

	if (game->objects.size <= 5) {
		add_objects(&game->objects, &game->objects_velocity);
	}

	da_for(&game->objects, i) {
		object_t *obj = &game->objects.items[i];
		const float speed = 1.5f;
		game->objects_velocity.items[i] = Vector3Scale(
			Vector3Normalize(
				Vector3Subtract(
					game->camera.position, obj->position)),
			speed
		);
	}

	gravity_apply(&game->player_velocity, delta_time);
}

static void add_objects(objects_t *objects, objects_velocity_t *objects_velocity)
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
		objects,
		object_create(
			.position = position,
			.size = size,
			.hitbox_position = position,
			.hitbox_size = size,
			.color = color,
		)
	 );
	da_append(objects_velocity, Vector3Zero());
	printf("objects.size = %lu\n", objects->size);
}
