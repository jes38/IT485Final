#include <stdlib.h>
#include "space.h"
#include "body.h"
#include "simple_logger.h"
#include "entity.h"

#include <glib.h>

struct Space_S
{
    GList *bodylist; /**<hash map of all bodies in space*/
    int steps;
    int stepstaken;
    float stepFactor;
};

Space *space_new()
{
    Space *space;
    space = (Space *)calloc(1,sizeof(struct Space_S));
    return space;
}

void touch_callback(void *data, void *context)
{
    Entity *me,*other;
    Body *obody;
    if ((!data)||(!context))return;
    me = (Entity *)data;
    obody = (Body *)context;
    if (entity_is_entity(obody->touch.data))
    {
        other = (Entity *)obody->touch.data;
        slog("%s is touching",me->name);
		slog("%s",other->name);

		if(me->uid == -1 || me->uid == -2 || me->uid == -3)
		{
			other->health -= 1;
			space_remove_body(gameSpace, &me->body);
			entity_free(me);
		}
    }
}

void space_set_steps(Space *space,int steps)
{
    if (!space)return;
    if (!steps)
    {
        slog("cannot support zero steps!");
        return;
    }
    space->steps = steps;
    space->stepFactor = 1.0/steps;
}

void space_remove_body(Space *space,Body *body)
{
    if (!space)return;
    if (!body)return;
    space->bodylist = g_list_remove(space->bodylist,body);
}

void space_add_body(Space *space,Body *body)
{
    if (!space)return;
    if (!body)return;
    space->bodylist = g_list_append(space->bodylist,body);
}


static void space_body_update(Space *space,Body *body)
{
    GList *it;
    Cube a,b;
    Body *other;
    Vec3D stepVector;
    Vec3D stepOffVector;
    
    if ((!body) || (body->_done))return;
    
    vec3d_scale(stepVector,body->velocity,space->stepFactor);
    vec3d_negate(stepOffVector,stepVector);
    
    vec3d_add(body->position,body->position,stepVector);
    
    a.x = body->position.x + body->bounds.x;
    a.y = body->position.y + body->bounds.y;
    a.z = body->position.z + body->bounds.z;
    a.w = body->bounds.w;
    a.h = body->bounds.h;
    a.d = body->bounds.d;
    
    for (it = space->bodylist;it != NULL;it = g_list_next(it))
    {
        if (!it->data)continue;
        if (it->data == body)continue;
        /*check for collision*/
        other = (Body *)it->data;
        vec3d_cpy(b,other->position);
        b.w = other->bounds.w;
        b.h = other->bounds.h;
        b.d = other->bounds.d;
        vec3d_add(b,b,other->bounds);

		//collision exceptions
		if(body->id == other->id){continue;}

		if(body->id == 0 && other->id == -1){continue;}
		else if(body->id == -1 && other->id == 0){continue;}
		else if(body->id == 0 && other->id == -2){continue;}
		else if(body->id == -2 && other->id == 0){continue;}
		else if(body->id > 0 && other->id == -3){continue;}
		else if(body->id == -3 && other->id > 0){continue;}

        if (cube_cube_intersection(a,b))
        {
            /*call touch functions*/
            /*back the fuck off*/
            vec3d_cpy(body->_stepOffVector,stepOffVector);
            body->_done = 1;
            body->_needsBackoff = 1;
            if (body->touch.function)
            {
				body->touch.function(body->touch.data,other);
            }
        }
    }
}

static void space_update(Space *space)
{
    GList *it;
    for (it = space->bodylist;it != NULL;it = g_list_next(it))
    {
        if (!it->data)continue;
        space_body_update(space,(Body*)it->data);
    }
    for (it = space->bodylist;it != NULL;it = g_list_next(it))
    {
        if (!it->data)continue;
        body_process((Body *)it->data);
    }
}

void space_do_step(Space *space)
{
    GList *it;
	
	if (!space)return;
    if (space->stepstaken == space->steps)
    {
        space->stepstaken = 0;
        for (it = space->bodylist;it != NULL;it = g_list_next(it))
        {
            if (!it->data)continue;
            body_reset((Body *)it->data);
        }
    }
    /*run one iteration of space update*/
    space_update(space);
    /*for each body, update it a little*/
    space->stepstaken++;
}

void space_free(Space *space)
{
    if (!space)return;
    free(space);
}

/*eol@eof*/
