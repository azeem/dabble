#ifndef __CANVAS_H__
#define __CANVAS_H__

#include <lua.h>

typdef struct canvas {
	SDL_Surface *screen;

	int ellipse_mode;
	int rect_mode;

	int no_stroke;
	Uint32 stroke_color;

	int no_fill;
	Uint32 fill_color;
} canas;

void open_canvaslib(lua_State *L);

#endif