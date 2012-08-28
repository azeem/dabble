#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <SDL/SDL.h>
#include "gfx.h"

int putpixel(lua_State *L);

static const struct luaL_Reg gfxlib[] = {
    {"putpixel", putpixel},
    {NULL, NULL}
};

void open_gfx_lib(lua_State *L) {
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "gfx");
	luaL_setfuncs(L, gfxlib, 0);
}

int putpixel(lua_State *L) {
	SDL_Surface *screen = luaL_checkudata(L, 1, NULL);
	int x = luaL_checkint(L, 2);
	int y = luaL_checkint(L, 3);
	int r = luaL_checkint(L, 4);
	int g = luaL_checkint(L, 5);
	int b = luaL_checkint(L, 6);
	
	Uint32 pixel = SDL_MapRGB(screen->format, r, g, b);
    int bpp = screen->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
    return 1;
}