#ifndef __SPACE_H__
#define __SPACE_H__

#include "body.h"

typedef struct Space_S Space;


Space *space_new();
Space *gameSpace;

void space_free(Space *space);
void touch_callback(void *data, void *context);
void space_set_steps(Space *space,int steps);
void space_do_step(Space *space);
void space_add_body(Space *space,Body *body);
void space_remove_body(Space *space,Body *body);

#endif
