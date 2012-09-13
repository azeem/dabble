#ifndef __TRANS_H__
#define __TRANS_H__

#include "dabble.h"

typedef struct DabbleMovement {
	Dabble dbl;
	SDL_Surface *buffer;
} DabbleMovement;

#define DBLMOV(dbl) ((DabbleMovement*)dbl)

DabbleType dbl_movementtype;

#endif