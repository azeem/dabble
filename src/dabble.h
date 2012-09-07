#ifndef __DABBLE_H__
#define __DABBLE_H__

#include <SDL/SDL.h>
#include <lua.h>

typedef struct Dabble {
	SDL_Surface *screen;
} Dabble;

void load_dabble(lua_State *L, const char *script_name, SDL_Surface *screen);
void run_dabble(lua_State *L);

#endif