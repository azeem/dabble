#ifndef __CANVAS_H__
#define __CANVAS_H__

#include <SDL/SDL.h>
#include <lua.h>

typedef struct Canvas {
	SDL_Surface *screen;

	int ellipse_mode;
	int rect_mode;

	int no_stroke;
	Uint32 stroke_color;

	int no_fill;
	Uint32 fill_color;
} Canvas;

void open_canvaslib(lua_State *L);
void new_canvas(lua_State *L, SDL_Surface *screen);


// Ellipse/Rectangle modes
#define CENTER  0
#define RADIUS  1
#define CORNER  2
#define CORNERS 3

#endif