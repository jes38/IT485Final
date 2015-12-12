#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "space.h"

/**
 * @brief initialize a level and clean up the old one
 * @param space The space to add the level's ships to
 * @param levNum The number of the level you wish to start (from 1 - 3)
 */

void startLevel(Space *space, int levNum);

#endif
