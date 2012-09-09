#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "trans.h"

static struct luaL_Reg movementlib[];

void
open_movementlib(lua_State *L) {
	luaL_newmetatable(L, "DabbleMovement");
	luaL_setfuncs(L, movementlib, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
}

static int
l_movement_init(lua_State *L) {
	//const char *script_name = luaL_checkstring(L, 1);
	SDL_Surface *screen = (SDL_Surface*)lua_touserdata(L, -1);

	DabbleMovement *dbl_mov = lua_newuserdata(L, sizeof(DabbleMovement));
	memset(dbl_mov, 0, sizeof(DabbleMovement));
	dbl_mov->dbl.screen = screen;
	luaL_setmetatable(L, "DabbleMovement");
	return 1;
}

static int
l_movement_noop(lua_State *L) {
	fprintf(stderr, "NOOP\n");
	return 0;
}

static struct luaL_Reg movementlib[] = {
	{"init", l_movement_init},
	{"setup", l_movement_noop},
	{"draw", l_movement_noop},
	{NULL, NULL}
};