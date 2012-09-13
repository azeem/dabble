#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include "SDL/SDL.h"
#include "lua.h"
#include "dabble.h"

typedef struct DabbleScript {
	Dabble dbl;
	int env;
} DabbleScript;

#define DBLSCRIPT(dbl) ((DabbleScript*)dbl)

struct DabbleType dbl_scripttype;

void open_dblscriptlib(lua_State *L);

#endif
