#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "canvas.h"
#include <SDL_gfxPrimitives.h>

#define CANVAS_SELF(L) ((Canvas*)luaL_checkudata(L, 1, "Canvas"))

static struct luaL_Reg canvaslib[];
static struct can_const_item {
	const char *name;
	int value;
} canvasconstants[]  = {
	{"CENTER",  CENTER},
	{"RADIUS",  RADIUS},
	{"CORNER",  CORNER},
	{"CORNERS", CORNERS},
	{NULL, 0},
};

void
open_canvaslib(lua_State *L) {
	luaL_newmetatable(L, "Canvas");
	luaL_setfuncs(L, canvaslib, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	// intialize the constants
	struct can_const_item *const_item = canvasconstants;
	while(const_item->name != NULL) {
		lua_pushvalue(L, -1);
		lua_pushinteger(L, const_item->value);
		lua_setfield(L, -2, const_item->name);
		const_item++;
	}
}

void
new_canvas(lua_State *L, SDL_Surface *screen) {
	luaL_getmetatable(L, "Canvas");
	lua_getfield(L, -1, "new");
	lua_remove(L, -2);
	lua_pushlightuserdata(L, screen);
	lua_call(L, 1, 1);
}

/* Begin canvas lua class functions */

int
l_canvas_new(lua_State *L) {
	SDL_Surface *screen = (SDL_Surface*)lua_touserdata(L, -1);
	Canvas *can = lua_newuserdata(L, sizeof(Canvas));
	memset(can, 0, sizeof(Canvas));
	can->screen = screen;
	luaL_setmetatable(L, "Canvas");
	return 1;
}

static Uint32
getcolorparam(lua_State *L) {
	int r = luaL_checkint(L, 2);
	int g = luaL_checkint(L, 3);
	int b = luaL_checkint(L, 4);
	int a = 255;
	int argc = lua_gettop(L);
	if(argc == 5) {
		a = luaL_checkint(L, 5);
	}
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

int
l_canvas_stroke(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	can->stroke_color = getcolorparam(L);
	can->no_stroke = 0;
	return 0;
}

int
l_canvas_fill(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	can->fill_color = getcolorparam(L);
	can->no_fill = 0;
	return 0;
}

int
l_canvas_no_stroke(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	can->no_stroke = 1;
	return 0;
}

int 
l_canvas_no_fill(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	can->no_fill = 1;
	return 0;
}

int
l_canvas_point(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	int x = luaL_checkint(L, 2);
	int y = luaL_checkint(L, 3);
	if(!can->no_stroke) {
		pixelColor(can->screen, x, y, can->stroke_color);
	}
	return 0;
}

int
l_canvas_line(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	int x1 = luaL_checkint(L, 2);
	int y1 = luaL_checkint(L, 3);
	int x2 = luaL_checkint(L, 4);
	int y2 = luaL_checkint(L, 5);
	if(!can->no_stroke) {
		lineColor(can->screen, x1, y1, x2, y2, can->stroke_color);
	}
	return 0;
}

int
l_canvas_ellipse(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	int a = luaL_checkint(L, 2);
	int b = luaL_checkint(L, 3);
	int c = luaL_checkint(L, 4);
	int d = luaL_checkint(L, 5);
	switch(can->ellipse_mode) {
		case RADIUS:
			break;
		case CORNER:
			c >>= 1;
			d >>= 1;
			a += c;
			b += d;
			break;
		case CORNERS:
			c = (c-a)>>1;
			d = (d-b)>>1;
			a += c;
			b += d;
			break;
		default:
		case CENTER:
			c >>= 1;
			d >>= 1;
			break;
	}
	if(!can->no_fill) {
		filledEllipseColor(can->screen, a, b, c, d, can->fill_color);
	}
	if(!can->no_stroke) {
		ellipseColor(can->screen, a, b, c, d, can->stroke_color);
	}
	return 0;
}

int
l_canvas_rect(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	int a = luaL_checkint(L, 2);
	int b = luaL_checkint(L, 3);
	int c = luaL_checkint(L, 4);
	int d = luaL_checkint(L, 5);
	switch(can->rect_mode) {
		case CORNER:
			a += c;
			c = a-c;
			d = b+d;
			break;
		case CORNERS: {
			int t = a;
			a = c;
			c = t;
			break;
		}
		case RADIUS:
			a += c;
			b -= d;
			c = a-(c<<1);
			d = b+(d<<1);
			break;
		default:
		case CENTER:
			a += (c>>1);
			b -= (d>>1);
			c = a-c;
			d = b+d;
			break;
	}
	if(!can->no_fill) {
		boxColor(can->screen, a, b, c, d, can->fill_color);
	}
	if(!can->no_stroke) {
		rectangleColor(can->screen, a, b, c, d, can->stroke_color);
	}
	return 0;
}

int
l_canvas_quad(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	Sint16 vx[] = {
		luaL_checkint(L, 2),
		luaL_checkint(L, 4),
		luaL_checkint(L, 6),
		luaL_checkint(L, 8)
	};
	Sint16 vy[] = {
		luaL_checkint(L, 3),
		luaL_checkint(L, 5),
		luaL_checkint(L, 7),
		luaL_checkint(L, 9)
	};
	if(!can->no_fill) {
		filledPolygonColor(can->screen, vx, vy, 4, can->fill_color);
	}
	if(!can->no_stroke) {
		polygonColor(can->screen, vx, vy, 4, can->stroke_color);
	}
	return 0;
}

int
l_canvas_ellipse_mode(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	int mode = luaL_checkint(L, 2);
	can->ellipse_mode = mode;
	return 0;
}

int
l_canvas_rect_mode(lua_State *L) {
	Canvas *can = CANVAS_SELF(L);
	int mode = luaL_checkint(L, 2);
	can->rect_mode = mode;
	return 0;
}

static struct luaL_Reg canvaslib[] = {
	{"new", l_canvas_new},
    {"stroke", l_canvas_stroke},
    {"no_stroke", l_canvas_no_stroke},
    {"fill", l_canvas_fill},
    {"no_fill", l_canvas_no_fill},
    {"point", l_canvas_point},
    {"line", l_canvas_line},
    {"ellipse_mode", l_canvas_ellipse_mode},
    {"ellipse", l_canvas_ellipse},
    {"rect_mode", l_canvas_rect_mode},
    {"rect", l_canvas_rect},
    {"quad", l_canvas_quad},
	{NULL, NULL}
};