#ifndef __SHIP_H__
#define __SHIP_H__

#define maxShips	100

#include "entity.h"
#include "space.h"

typedef struct Ship_S
{
	int inuse;
	int shipID; //0 is player
	int shipType; //what kind of ship is it
	//Vec3D pos;  not sure if we need this
	Vec3D vel;
    Vec3D acc;
    float rot;

	Vec3D turrOff;
	Vec3D gunOff;
	
	Entity *hull;
	Entity *turret;
	Entity *gun;
}Ship;

Ship *shipList;

float shipVel; //players ship velocity
float shipRot; //player's rotation
float turretRot; //player's turret rotation
float gunElev; //player's gun elevation
int numShips; //does not include player
float realTurrRot;
float targDst; //distance of target from player (for HUD)
float enVec; //vector of target from player (for HUD)

/**
 * @brief add a ship to the ship list
 */
Ship *newShip();

/**
 * @brief initialize the ship list
 */
void initShips();

/**
 * @brief free a single ship
 * @param ship the ship to free
 */
void freeShip(Space *space, Ship *ship);

/**
 * @brief remove all ships from the game
 * @param notPlayer should the function remove the player (0 = yes, 1 = no)
 */
void freeAllShips(Space *space, int notPlayer);

/**
 * @brief Update ship velocity and the velocity of the hull ONLY
 * @param ship The ship to update
 */
void updateShipPos(Ship *ship);

/**
 * @brief Update the turret and gun of the ship. Must be done after the hull's position is set
 * @param ship The components of which ship to update
 */
void componentInherit(Ship *ship);

/**
 * @brief Update all ships
 */
void updateAllShipPos();

/**
 * @brief Update all components
 */
void updateAllShipComp();

/**
 * @brief take player input and apply it to this ship
 * @param ship Pointer to the player's ship
 */
void takeShipInput(Ship *ship);

/**
 * @brief spawn a new ship into the world, and return a pointer
 * @param space The space to spawn the ship into
 * @param spawnPt The point where the ship will spawn 
 * @param shipType The type of ship to spawn 
 */
Ship *spawnShip(Space *space, Vec3D spawnPt, int shipType, float rotation);

/**
 * @brief search ship list for next ship in use
 * @param start Point to start searching in ship list 
 */
int scanForNext(int start);

/**
 * @brief return pointer to a ship based on id
 * @param id the id of the requested ship
 */
Ship *returnShip(int id);

/**
 * @brief AI targeting function
 * @param the AI ship that's doing the targeting
 */
void enemyTarget(Ship *ship);

/**
 * @brief similar to spawning a ship, but spawns an island
 * @param space space to spawn the island
 * @param spawnPt where to spawn the island
 */
Ship *spawnIsland(Space *space, Vec3D spawnPt);

/**
 * @brief update the values for the HUD
 * @param player pointer to player
 * @param selected pointer to selected ship
 */
void HUDupdate(Ship *player, Ship *selected);

/**
 * @brief swap the players ship to another ship
 * @param curPlayer pointer to (current) player
 * @param selected pointer to the ship that will be the player
 */
void swapShips(Ship *curPlayer, Ship *selected);

#endif
