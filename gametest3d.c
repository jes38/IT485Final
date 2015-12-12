/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "space.h"
#include "ship.h"
#include "weapon.h"
#include "level.h"

void set_camera(Vec3D position, Vec3D rotation);

Entity *newCube(Vec3D position,const char *name)
{
    Entity * ent;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    ent->objModel = obj_load("models/cube.obj");
    ent->texture = LoadSprite("models/cube_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
    mgl_callback_set(&ent->body.touch,touch_callback,ent);
	ent->body.id = -3;
    return ent;
}

int main(int argc, char *argv[])
{
    int i;
    float r = 0;
    Space *space;
    Entity *cube1,*cube2;
    char bGameLoopRunning = 1;
    Vec3D cameraPosition = {0,5,0};
    Vec3D cameraRotation = {180,0,180};
    SDL_Event e;
    Obj *bgobj;
    Sprite *bgtext;

	//my variables
	Ship *playerShip;
	Ship *selectedShip;
	int specMode;
	int editormode;
	int editorselection;
	int levelselected;
    
    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d",33) != 0)
    {
        return -1;
    }
    model_init();
    obj_init();
    entity_init(255);
	initShips();
    
    bgobj = obj_load("models/mountainvillage.obj");
    bgtext = LoadSprite("models/mountain_text.png",1024,1024);
    
    space = space_new();
    space_set_steps(space,100);

	//my variables
	specMode = 0;
	numShips = 0;
	shipVel = 0;
	shipRot = 0;
	turretRot = 0;
	gunElev = 0;
	realTurrRot = 0;
	editormode = 0; //0=editorOff 1=selection 2=place
	editorselection = 0;
	levelselected = 0;

	playerShip = spawnShip(space, vec3d(0,0,0), 1, 0);
	selectedShip = playerShip;

    while (bGameLoopRunning)
    {
		updateAllShipPos();
		for (i = 0; i < 100;i++)
        {
			space_do_step(space);
        }
		updateAllShipComp();
		applyGrav();
		if(specMode == 0 && editormode == 0)
		{
			cameraPosition.x = playerShip->hull->body.position.x;
			cameraPosition.y = (playerShip->hull->body.position.y + 5);
			cameraPosition.z = playerShip->hull->body.position.z;
		}
		else if (editormode > 0)
		{
			cameraPosition.x = selectedShip->hull->body.position.x;
			cameraPosition.z = (selectedShip->hull->body.position.z - 70);
		}

        while ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT)
            {
                bGameLoopRunning = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    bGameLoopRunning = 0;
                }
				else if (e.key.keysym.sym == SDLK_0 && editormode == 0)
				{
					editormode = 1;

					playerShip->hull->body.position.x = 0;
					playerShip->hull->body.position.z = 0;
					cameraPosition.x = playerShip->hull->body.position.x;
					cameraPosition.y = (playerShip->hull->body.position.y + 50);
					cameraPosition.z = (playerShip->hull->body.position.z - 70);
					cameraRotation.x = 150;
					cameraRotation.y = 0;
					cameraRotation.z = 180;

					freeAllShips(1);
				}
				else if (e.key.keysym.sym == SDLK_0 && editormode > 0)
				{
					editormode = 0;
					editorselection = 0;
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_1)
				{
					startLevel(space, 1);
					levelselected = 1;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_2)
				{
					startLevel(space, 2);
					levelselected = 2;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_3)
				{
					startLevel(space, 3);
					levelselected = 3;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_4)
				{
					startLevel(space, 4);
					levelselected = 4;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_5)
				{
					startLevel(space, 5);
					levelselected = 5;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_6)
				{
					startLevel(space, 6);
					levelselected = 6;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_7)
				{
					startLevel(space, 7);
					levelselected = 7;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_8)
				{
					startLevel(space, 8);
					levelselected = 8;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				else if (e.key.keysym.sym == SDLK_9)
				{
					startLevel(space, 9);
					levelselected = 9;
					playerShip = returnShip(0);
					selectedShip = playerShip;
				}
				

				else if (editormode == 0)
				{
					if (e.key.keysym.sym == SDLK_LEFT)
					{
						cameraRotation.y -= 1;
					}
					else if (e.key.keysym.sym == SDLK_RIGHT)
					{
						cameraRotation.y += 1;
					}
					else if (e.key.keysym.sym == SDLK_UP)
					{
						cameraRotation.x += 1;
					}
					else if (e.key.keysym.sym == SDLK_DOWN)
					{
						cameraRotation.x -= 1;
					}
					else if (e.key.keysym.sym == SDLK_p)
					{
						if(specMode == 0){specMode = 1;}
						else {specMode = 0;}
					}

					else if (specMode)	
					{
						if (e.key.keysym.sym == SDLK_SPACE)
						{
							cameraPosition.z++;
						}
						else if (e.key.keysym.sym == SDLK_z)
						{
							cameraPosition.z--;
						}
						else if (e.key.keysym.sym == SDLK_w)
						{
							vec3d_add(
								cameraPosition,
								cameraPosition,
								vec3d(
									-sin(cameraRotation.z * DEGTORAD),
									cos(cameraRotation.z * DEGTORAD),
									0
								));
						}
						else if (e.key.keysym.sym == SDLK_s)
						{
							vec3d_add(
								cameraPosition,
								cameraPosition,
								vec3d(
									sin(cameraRotation.z * DEGTORAD),
									-cos(cameraRotation.z * DEGTORAD),
									0
								));
						}
						else if (e.key.keysym.sym == SDLK_d)
						{
							vec3d_add(
								cameraPosition,
								cameraPosition,
								vec3d(
									cos(cameraRotation.z * DEGTORAD),
									sin(cameraRotation.z * DEGTORAD),
									0
								));
						}
						else if (e.key.keysym.sym == SDLK_a)
						{
							vec3d_add(
								cameraPosition,
								cameraPosition,
								vec3d(
									-cos(cameraRotation.z * DEGTORAD),
									-sin(cameraRotation.z * DEGTORAD),
									0
								));
						}
					}

					else if(!specMode)
					{
						if (e.key.keysym.sym == SDLK_w && shipVel < 0.4)
						{
							shipVel += 0.1;
						}
						else if (e.key.keysym.sym == SDLK_s && shipVel > 0.05)
						{
							shipVel -= 0.1;
						}
						else if (e.key.keysym.sym == SDLK_d)
						{
							shipRot += 0.5;
							if(shipRot >= 360){shipRot -= 360;}
							playerShip->rot = shipRot;
						}
						else if (e.key.keysym.sym == SDLK_a)
						{
							shipRot -= 0.5;
							if(shipRot < 0){shipRot += 360;}
							playerShip->rot = shipRot;
						}
						else if (e.key.keysym.sym == SDLK_c && turretRot < 135)
						{
							turretRot += 1;
						}
						else if (e.key.keysym.sym == SDLK_z && turretRot > -135)
						{
							turretRot -= 1;
						}
						else if (e.key.keysym.sym == SDLK_k && gunElev < 50)
						{
							gunElev += 0.5;
						}
						else if (e.key.keysym.sym == SDLK_m && gunElev > -5)
						{
							gunElev -= 0.5;
						}
						else if (e.key.keysym.sym == SDLK_SPACE)
						{
							fireBullet(space, playerShip->gun->body.position, realTurrRot, gunElev, 0.5, -1);
						}
						else if (e.key.keysym.sym == SDLK_b)
						{
							fireBullet(space, playerShip->hull->body.position, shipRot, 0, 0, -2);
						}
					}
				}

				else if (editormode > 0)
				{
						if (e.key.keysym.sym == SDLK_p) //p for place
						{
							Ship *newShip = spawnShip(space, vec3d((selectedShip->hull->body.position.x + 10), 0, (selectedShip->hull->body.position.z + 10)), 1, 0);
							selectedShip = newShip;
							editorselection = selectedShip->shipID;
						}
						else if (e.key.keysym.sym == SDLK_BACKSPACE && levelselected != 0)
						{
							saveLevel(levelselected);
						}
						else if (e.key.keysym.sym == SDLK_UP)
						{
							editorselection += 1;
							editorselection = scanForNext(editorselection);
							selectedShip = returnShip(editorselection);
						}
						else if (e.key.keysym.sym == SDLK_r && selectedShip->shipID != 0) //r for remove
						{
							freeShip(selectedShip);
							
							editorselection += 1;
							editorselection = scanForNext(editorselection);
							selectedShip = returnShip(editorselection);
						}
						else if (e.key.keysym.sym == SDLK_w)
						{
							selectedShip->hull->body.position.z += 1;
						}
						else if (e.key.keysym.sym == SDLK_s)
						{
							selectedShip->hull->body.position.z -= 1;
						}
						else if (e.key.keysym.sym == SDLK_d)
						{
							selectedShip->hull->body.position.x -= 1;
						}
						else if (e.key.keysym.sym == SDLK_a)
						{
							selectedShip->hull->body.position.x += 1;
						}
						else if (e.key.keysym.sym == SDLK_RIGHT)
						{
							selectedShip->rot += 1;
						}
						else if (e.key.keysym.sym == SDLK_LEFT)
						{
							selectedShip->rot -= 1;
						}
				}
            }
        }
		 
        graphics3d_frame_begin();
        
        glPushMatrix();
        set_camera(
            cameraPosition,
            cameraRotation);
        
        entity_draw_all();

        if (r > 360)r -= 360;
        glPopMatrix();
        /* drawing code above here! */
        graphics3d_next_frame();
    } 
    return 0;
}

void set_camera(Vec3D position, Vec3D rotation)
{
    glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position.x,
                 -position.y,
                 -position.z);
}

/*eol@eof*/