#include <stdlib.h>
#include "level.h"
#include "ship.h"
#include "vector.h"
#include "space.h"

void startLevel(Space *space, int levNum)
{
	freeAllShips(1);

	if (levNum == 1)
	{
		spawnShip(space, vec3d(-12,0,75), 1);
		spawnShip(space, vec3d(0,0,75), 1);
		spawnShip(space, vec3d(12,0,75), 1);
	}
	if (levNum == 2)
	{
		spawnShip(space, vec3d(-12,0,75), 2);
		spawnShip(space, vec3d(0,0,75), 2);
		spawnShip(space, vec3d(12,0,75), 2);
	}
	if (levNum == 3)
	{
		spawnShip(space, vec3d(-12,0,75), 3);
		spawnShip(space, vec3d(0,0,75), 3);
		spawnShip(space, vec3d(12,0,75), 3);
	}
}
