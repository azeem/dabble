#ifndef __TRANS_H__
#define __TRANS_H__

#include "dabble.h"

typedef struct DabbleMovement {
	Dabble dbl;
} DabbleMovement;

void open_movementlib(lua_State *L);

#endif