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

	char *filepath = NULL;
	if(levNum == 1){filepath = "levels/1.txt";}
	else if(levNum == 2){filepath = "levels/2.txt";}
	else if(levNum == 3){filepath = "levels/3.txt";}
	else if(levNum == 4){filepath = "levels/4.txt";}
	else if(levNum == 5){filepath = "levels/5.txt";}
	else if(levNum == 6){filepath = "levels/6.txt";}
	else if(levNum == 7){filepath = "levels/7.txt";}
	else if(levNum == 8){filepath = "levels/8.txt";}
	else if(levNum == 9){filepath = "levels/9.txt";}

	fileptr = fopen(filepath,"r");

	if (!fileptr)
    {
        fprintf(stderr,"unable to open file: %s\n",filepath);
		return;
    }

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
