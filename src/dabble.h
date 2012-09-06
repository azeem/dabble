#ifndef __DABBLE_H__
#define __DABBLE_H__

#include "SDL/SDL.h"
#include "lua.h"

// #define DBL_LUA_PATH "./lua/?.lua"

typedef struct dabble_s {
	SDL_Surface *screen;
	
	int ellipse_mode;
	int rect_mode;

	int no_stroke;
	Uint32 stroke_color;

	int no_fill;
	Uint32 fill_color;
} Dabble;

void dbl_init_lua(lua_State *L);
void dbl_load_dabble(lua_State *L, const char *script_name);
void dbl_run_dabble(lua_State *L);

// Ellipse modes
#define CENTER  0
#define RADIUS  1
#define CORNER  2
#define CORNERS 3

#endif
