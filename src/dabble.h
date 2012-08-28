#ifndef __DABBLE_H__
#define __DABBLE_H__

#include "SDL/SDL.h"
#include "lua.h"

// #define DBL_LUA_PATH "./lua/?.lua"

typedef struct dabble_s {
	SDL_Surface *screen;
	lua_State *L;
	int dbl_lib_ref;
} Dabble;

Dabble *new_dabble(SDL_Surface *screen);
void free_dabble(Dabble *dbl);
int do_script(Dabble *dbl, const char *name);

#endif
