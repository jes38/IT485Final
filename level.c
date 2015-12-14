#include <stdlib.h>
#include <stdio.h>
#include "level.h"
#include "ship.h"
#include "vector.h"
#include "space.h"

void startLevel(Space *space, int levNum)
{
	FILE *fileptr;
	char buf[255];

	int type = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	float rot = 0;

	char filepath[512];
	if(levNum >= 1 && levNum <= 9)
	{
		sprintf(filepath,"levels/%d.txt", levNum);
	}
	else
	{
		sprintf(filepath,"Filepath error");
	}
	
	fileptr = fopen(filepath,"r");

	if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filepath);
		return;
    }

	freeAllShips(space, 0);

	while (fscanf(fileptr,"%s",buf) != EOF)
    {
        if (!strcmp(buf,"Ship:"))
        {
			fscanf(fileptr, " %i %f %f %f %f", &type, &x, &y, &z, &rot);
			spawnShip(space, vec3d(x,y,z), type, rot);
        }
		else if (!strcmp(buf,"Island:"))
        {
			fscanf(fileptr, " %f %f %f", &x, &y, &z);
			spawnIsland(space, vec3d(x,y,z));
        }
    }

    fclose(fileptr);
}

void saveLevel(int levNum)
{
	int i;
	FILE *fileptr;
	
	char filepath[512];
	if(levNum >= 1 && levNum <= 9)
	{
		sprintf(filepath,"levels/%d.txt", levNum);
	}
	else
	{
		sprintf(filepath,"Filepath error");
	}

    fileptr = fopen(filepath,"w");

	if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filepath);
    }

    for (i = 0;i < maxShips;i++)
    {
		if (shipList[i].inuse && shipList[i].shipID >= 0)
        {
            Ship *ship = &shipList[i];
			fprintf(fileptr,"Ship: %i %f %f %f %f\n", ship->shipType, ship->hull->body.position.x, ship->hull->body.position.y, ship->hull->body.position.z, ship->rot);
        }
		else if (shipList[i].inuse && shipList[i].shipID < 0)
        {
            Ship *ship = &shipList[i];
			fprintf(fileptr,"Island: %f %f %f\n", ship->hull->body.position.x, ship->hull->body.position.y, ship->hull->body.position.z);
        }
    }

    fclose(fileptr);
}
