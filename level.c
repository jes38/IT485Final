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
	int idx = 0;

	int type = 0;
	float x = 0;
	float y = 0;
	float z = 0;
	float rot = 0;

	char *filepath = NULL;
	if(levNum == 1){filepath = "1";}
	else if(levNum == 2){filepath = "2";}
	else if(levNum == 3){filepath = "3";}
	else if(levNum == 4){filepath = "4";}
	else if(levNum == 5){filepath = "5";}
	else if(levNum == 6){filepath = "6";}
	else if(levNum == 7){filepath = "7";}
	else if(levNum == 8){filepath = "8";}
	else if(levNum == 9){filepath = "9";}

	fileptr = fopen(filepath,"r");
	
	freeAllShips(0);

	while (fscanf(fileptr,"%s",buf) != EOF)
    {
        if (!strcmp(buf,"Ship:"))
        {
			fscanf(fileptr, "%d %d %d %d %d", &type, &x, &y, &z, &rot);
			spawnShip(space, vec3d(x,y,z), type, rot);
        }
    }

    fclose(fileptr);
}
