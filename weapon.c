#include <stdlib.h>
#include <math.h>
#include "weapon.h"
#include "ship.h"
#include "vector.h"
#include "entity.h"

void fireBullet(Space *space, Vec3D spawn, float angle, float elev, float bullVel, int id)
{
	Entity *bullet;
	bullet = entity_new();

	bullet->health = 300;
	bullet->objModel = obj_load("models/cube.obj");
	bullet->texture = LoadSprite("models/cube_text.png",1024,1024);
	vec3d_cpy(bullet->body.position,vec3d(spawn.x, spawn.y, spawn.z));
	cube_set(bullet->body.bounds,-0.1,-0.1,-0.1,0.2,0.2,0.2);
	bullet->scale = vec3d(0.1,0.1,0.1);
	mgl_callback_set(&bullet->body.touch,touch_callback,bullet);
	space_add_body(space,&bullet->body);

	bullet->body.velocity.x = (bullVel * -sin(angle * DEGTORAD));
	bullet->body.velocity.z = (bullVel * cos(angle * DEGTORAD));

	bullet->body.velocity.y = (bullVel * sin(elev * DEGTORAD));

	bullet->uid = id;
	bullet->body.id = id;
}

void AIfire(Ship *ship)
{
	fireAIBullet(gameSpace, ship->gun->body.position, ship->turret->rotation.y, -2, 0.5, -3);
}

void fireAIBullet(Space *space, Vec3D spawn, float angle, float elev, float bullVel, int id)
{
	Entity *bullet;
	bullet = entity_new();

	bullet->health = 300;
	bullet->objModel = obj_load("models/cube.obj");
	bullet->texture = LoadSprite("models/cube_text.png",1024,1024);
	vec3d_cpy(bullet->body.position,vec3d(spawn.x, spawn.y, spawn.z));
	cube_set(bullet->body.bounds,-0.1,-0.1,-0.1,0.2,0.2,0.2);
	bullet->scale = vec3d(0.1,0.1,0.1);
	mgl_callback_set(&bullet->body.touch,touch_callback,bullet);
	space_add_body(space,&bullet->body);

	bullet->body.velocity.x = (-1 * bullVel * -sin(angle * DEGTORAD));
	bullet->body.velocity.z = (bullVel * cos(angle * DEGTORAD));

	bullet->body.velocity.y = (bullVel * sin(elev * DEGTORAD));

	bullet->uid = id;
	bullet->body.id = id;
}
