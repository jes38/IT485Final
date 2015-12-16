#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "ship.h"
#include "entity.h"

int AIcounter; //determines when the AI should fire

/**
 * @brief fire a bullet from anywhere
 * @param space The space to add the bullet to.
 * @param spawn The spawn point of the bullet
 * @param angle The vector on the x,z plane that the bullet will be fired
 * @param elev The up/down angle the bullet will be fired
 * @param bullVel The velocity of the bullet
 * @param id The type of bullet (1 = cannon, 2 = depth charge)
 */
void fireBullet(Space *space, Vec3D spawn, float angle, float elev, float bullVel, int id);

/**
 * @brief after AI targets player, fires bullet
 * @param ship the ship that's firing
 */
void AIfire(Ship *ship);

/**
 * @brief fire an AI bullet from anywhere
 * @param space The space to add the bullet to.
 * @param spawn The spawn point of the bullet
 * @param angle The vector on the x,z plane that the bullet will be fired
 * @param elev The up/down angle the bullet will be fired
 * @param bullVel The velocity of the bullet
 * @param id The type of bullet (3 = AI bullet)
 */
void fireAIBullet(Space *space, Vec3D spawn, float angle, float elev, float bullVel, int id);

#endif
