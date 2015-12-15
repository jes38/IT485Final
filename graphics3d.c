#include "graphics3d.h"
#include "simple_logger.h"
#include "shader.h"
#include "sprite.h"
#include "vector.h"
#include "ship.h"
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>

static SDL_GLContext __graphics3d_gl_context;
static SDL_Window  * __graphics3d_window = NULL;
static GLuint        __graphics3d_shader_program;
static Uint32        __graphics3d_frame_delay = 33;

void graphics3d_close();

GLuint graphics3d_get_shader_program()
{
    return __graphics3d_shader_program;
}

void graphics3d_setup_default_light();

Sprite *numbers;
Sprite *enemyVec;
Sprite *elev;
Sprite *targDist;
Sprite *turrRot;
Sprite *negative;

int graphics3d_init(int sw,int sh,int fullscreen,const char *project,Uint32 frameDelay)
{
    const unsigned char *version;
    GLenum glew_status;
        
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        slog("failed to initialize SDL!");
        return -1;
    }
    atexit(SDL_Quit);
    __graphics3d_frame_delay = frameDelay;
    
    __graphics3d_window = SDL_CreateWindow(project?project:"gametest3d",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              sw, sh,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    
    
    __graphics3d_gl_context = SDL_GL_CreateContext(__graphics3d_window);
    if (__graphics3d_gl_context == NULL)
    {
        slog("There was an error creating the OpenGL context!\n");
        return -1;
    }
    
    version = glGetString(GL_VERSION);
    if (version == NULL) 
    {
        slog("There was an error creating the OpenGL context!\n");
        return -1;
    }
    
    SDL_GL_MakeCurrent(__graphics3d_window, __graphics3d_gl_context);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    //MUST make a context AND make it current BEFORE glewInit()!
    glewExperimental = GL_TRUE;
    glew_status = glewInit();
    if (glew_status != 0) 
    {
        slog("Error: %s", glewGetErrorString(glew_status));
        return -1;
    }
    
    
    __graphics3d_shader_program = BuildShaderProgram("shaders/vs1.glsl", "shaders/fs1.glsl");
    if (__graphics3d_shader_program == -1)
    {
        return -1;
    }
    
    
    glViewport(0,0,sw, sh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    /*view angle, aspect ratio, near clip distance, far clip distance*/
    /*TODO: put near/far clip in graphics view config*/
    gluPerspective( 40, (float)sw / (float)sh, .01, 2000.0f);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    glClear( 1 );
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /*Enables alpha testing*/
/*    glAlphaFunc(GL_GREATER,0);
    glEnable(GL_ALPHA_TEST);*/
    
    graphics3d_setup_default_light();
    atexit(graphics3d_close);
    return 0;
}

void graphics3d_close()
{
    SDL_GL_DeleteContext(__graphics3d_gl_context);
}

void graphics3d_frame_begin()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glPushMatrix();
}

void graphics3d_next_frame()
{
    static Uint32 then = 0;
    Uint32 now;
    glPopMatrix();
    SDL_GL_SwapWindow(__graphics3d_window);
    now = SDL_GetTicks();
    if ((now - then) < __graphics3d_frame_delay)
    {
        SDL_Delay(__graphics3d_frame_delay - (now - then));        
    }
    //slog("ticks passed this frame: %i",(now - then));
    then = now;
}

void graphics3d_setup_default_light()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { -10.0, -10.0, 10.0, 0.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    
    GLfloat light1_ambient[] = { 1.2, 1.2, 1.2, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_position[] = { -20.0, 2.0, 1.0, 1.0 };
    GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };
    
    GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    
    //glShadeModel (GL_SMOOTH);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.9);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.7);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 80.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    
}

//////////////////////////
//UI GRAPHICS BELOW HERE//
//////////////////////////
void DrawUI()
{
	int i;
	int o;
	char numString[25];
	char num1;
	char num2;
	char num3;
	char num4;
	for(i = 0;i < 8;i++)
	{
		glMatrixMode(GL_PROJECTION);

		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, 1024, 0.0, 768, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_LIGHTING);

		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
	
		if(i < 4)
		{
			drawBoxes(i);
		}
		else
		{
			if(i == 4){sprintf(numString, "%f", turretRot);}
			else if(i == 5){sprintf(numString, "%f", targDst);}
			else if(i == 6){sprintf(numString, "%f", gunElev);}
			else if(i == 7){sprintf(numString, "%f", enVec);}

			for(o = 0;o < 4;o++)
			{
				if(o==0)
				{
					num1 = numString[o];
					//num1 = '-';
				}
				else if(o==1)
				{
					num2 = '.';
					num2 = numString[o];
					//num2 = '2';
					if(num2 == '.')
					{
						drawNums((i - 4), num1, NULL, NULL, NULL);
						o = 5;
					}
				}
				else if(o==2)
				{
					num3 = '.';
					num3 = numString[o];
					//num3 = '3';
					if(num3 == '.')
					{
						drawNums((i - 4), num1, num2, NULL, NULL);
						o = 5;
					}
				}
				else if(o==3)
				{
					num4 = '.';
					num4 = numString[o];
					//num4 = '4';
					if(num4 == '.')
					{
						drawNums((i - 4), num1, num2, num3, NULL);
						o = 5;
					}
					else
					{
						drawNums((i - 4), num1, num2, num3, num4);
					}
				}
			}
		}

		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
	}
}

void drawBoxes(int i)
{
	GLuint texture;

	float xsize = 500;
	float ysize = 50;
	float x = 0;
	float y = 750;
	
	if(i == 0){texture = turrRot->texture;}
	else if(i == 1){texture = targDist->texture;}
	else if(i == 2){texture = elev->texture;}
	else if(i == 3){texture = enemyVec->texture;}

	glBindTexture(GL_TEXTURE_2D, texture);
	y -= (ysize * i);
	glTranslatef(x, y, 0);

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); 
		glVertex3f(0, 0, 0);

		glTexCoord2f(1, 0); 
		glVertex3f(xsize, 0, 0);

		glTexCoord2f(1, 1); 
		glVertex3f(xsize, -ysize, 0);

		glTexCoord2f(0, 1); 
		glVertex3f(0, -ysize, 0);
	glEnd();
}

void drawNums(int i, char num1, char num2, char num3, char num4)
{
	float number;
	float xsize = 18;
	float ysize = 37.5;
	float x = 202;
	float y = 744;

	glBindTexture(GL_TEXTURE_2D, numbers->texture);
	y -= (50 * i);
	glTranslatef(x, y, 0);

	number = charCheck(num1);
	if(number == -1){number = 10;}
	glBegin(GL_QUADS);
		glTexCoord2f(number/11, 0); 
		glVertex3f(0, 0, 0);

		glTexCoord2f((number + 1)/11, 0); 
		glVertex3f(xsize, 0, 0);

		glTexCoord2f((number + 1)/11, 1); 
		glVertex3f(xsize, -ysize, 0);

		glTexCoord2f(number/11, 1); 
		glVertex3f(0, -ysize, 0);
	glEnd();

	if(num2 != NULL){number = charCheck(num2);}
	else{return;}
	hudReset();
	glBindTexture(GL_TEXTURE_2D, numbers->texture);
	x += 15;
	glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(number/11, 0); 
		glVertex3f(0, 0, 0);

		glTexCoord2f((number + 1)/11, 0); 
		glVertex3f(xsize, 0, 0);

		glTexCoord2f((number + 1)/11, 1); 
		glVertex3f(xsize, -ysize, 0);

		glTexCoord2f(number/11, 1); 
		glVertex3f(0, -ysize, 0);
	glEnd();

	if(num3 != NULL){number = charCheck(num3);}
	else{return;}
	hudReset();
	glBindTexture(GL_TEXTURE_2D, numbers->texture);
	x += 15;
	glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(number/11, 0); 
		glVertex3f(0, 0, 0);

		glTexCoord2f((number + 1)/11, 0); 
		glVertex3f(xsize, 0, 0);

		glTexCoord2f((number + 1)/11, 1); 
		glVertex3f(xsize, -ysize, 0);

		glTexCoord2f(number/11, 1); 
		glVertex3f(0, -ysize, 0);
	glEnd();

	if(num4 != NULL){number = charCheck(num4);}
	else{return;}
	hudReset();
	glBindTexture(GL_TEXTURE_2D, numbers->texture);
	x += 15;
	glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(number/11, 0); 
		glVertex3f(0, 0, 0);

		glTexCoord2f((number + 1)/11, 0); 
		glVertex3f(xsize, 0, 0);

		glTexCoord2f((number + 1)/11, 1); 
		glVertex3f(xsize, -ysize, 0);

		glTexCoord2f(number/11, 1); 
		glVertex3f(0, -ysize, 0);
	glEnd();
}

float charCheck(char num)
{
	if(num == '-'){return -1;}
	if(num == '0'){return 0;}
	if(num == '1'){return 1;}
	if(num == '2'){return 2;}
	if(num == '3'){return 3;}
	if(num == '4'){return 4;}
	if(num == '5'){return 5;}
	if(num == '6'){return 6;}
	if(num == '7'){return 7;}
	if(num == '8'){return 8;}
	if(num == '9'){return 9;}
}

void hudReset()
{
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 1024, 0.0, 768, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);

	glColor3f(1,1,1);
	glEnable(GL_TEXTURE_2D);
}

void initTextSprites()
{
	numbers = LoadSprite("font/numbers.png", 396, 75);
	elev = LoadSprite("font/gunElev.png", 1000, 100);
	enemyVec = LoadSprite("font/enemyVec.png", 1000, 100);
	targDist = LoadSprite("font/targDist.png", 1000, 100);
	turrRot = LoadSprite("font/turrRot.png", 1000, 100);
}

void HUDupdate(Ship *player, Ship *selected)
{
	float x, z, xdist, zdist, angle;
	
	x = player->hull->body.position.x;
	z = player->hull->body.position.z;
	xdist = (selected->hull->body.position.x - x);
	zdist = (selected->hull->body.position.z - z);
	if (zdist < 0)
	{
		zdist *= -1;
		xdist *= -1;
		angle = (atan(xdist/zdist) * RADTODEG);
	}
	else
	{
		angle = (atan(xdist/zdist) * RADTODEG);
	}

	targDst = sqrt((zdist * zdist) + (xdist * xdist));
	enVec = angle;
}

/*eol@eof*/
