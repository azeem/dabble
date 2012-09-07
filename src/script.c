#include <stdlib.h>
#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <time.h>
#include "script.h"
#include "canvas.h"

static struct luaL_Reg dblscriptlib[];

void
open_dblscriptlib(lua_State *L) {
	// load dabble class
	luaL_newmetatable(L, "DabbleScript");
	luaL_setfuncs(L, dblscriptlib, 0);

	// init dbl_loaded table that stores closures of loaded scripts
	lua_newtable(L);
	lua_setglobal(L, "dbl_loaded");

	// seed random
	lua_getglobal(L, "math");
	lua_getfield(L, -1, "randomseed");
	lua_pushinteger(L, time(NULL));
	lua_call(L, 1, 0);
}

/* Begin Dabble Lua class functions */

static int
l_script_init(lua_State *L) {
	// stack: script_name, screen
	const char *script_name = luaL_checkstring(L, 1);
	SDL_Surface *screen = (SDL_Surface*)lua_touserdata(L, -1);

	lua_getglobal(L, "dbl_loaded");
	lua_getfield(L, -1, script_name);
	if(lua_isnil(L, -1)) {
		// search the script in the path
		lua_getglobal(L, "package");
		lua_getfield(L, -1, "searchpath");
		lua_pushstring(L, script_name);
		lua_pushstring(L, DBL_LUA_PATH);
		lua_call(L, 2, 1);
		if(lua_isnil(L, -1)) {
			lua_pushfstring(L, "Dabble script '%s' not found", script_name);
			return 2;
		}

		// load the script
		int load_status = luaL_loadfile(L, lua_tostring(L, -1));
		if(load_status != LUA_OK) {
			lua_pushnil(L);
			lua_pushfstring(L, "Dabble script '%s' could not be loaded", script_name);
			return 2;
		}

		// stack: script_function, script_path, nil, dbl_loaded
		lua_pushvalue(L, -1);
		lua_setfield(L, -4, script_name);
	}

	// create a new dabble object
	DabbleScript *dbl_script = lua_newuserdata(L, sizeof(DabbleScript));
	memset(dbl_script, 0, sizeof(DabbleScript));
	dbl_script->dbl.screen = screen;
	luaL_setmetatable(L, "DabbleScript");
	// setup the uservalue table for the dabble object
	lua_newtable(L);
	new_canvas(L, screen);
	lua_setfield(L, -2, "canvas");
	lua_setuservalue(L, -2);

	// set _ENV of script and run it
	//stack: object, script_function
	lua_pushvalue(L, -1);
	lua_setupvalue(L, -3, 1);
	lua_pushvalue(L, -2);
	lua_call(L, 0, 0);
	return 1;
}

static int
l_script_index(lua_State *L) {
	// object, key
	lua_getuservalue(L, -2);
	lua_pushvalue(L, -2);
	lua_rawget(L, -2);
	if(!lua_isnoneornil(L, -1)) {
		return 1;
	}

	lua_pop(L, 2);

	const char *key = lua_tostring(L, -1);
	const DabbleScript *dbl_script = (DabbleScript *)lua_touserdata(L, -2);
	if(strcmp(key, "height") == 0) {
		lua_pushinteger(L, dbl_script->dbl.screen->h);
		return 1;
	}
	else if(strcmp(key, "width") == 0) {
		lua_pushinteger(L, dbl_script->dbl.screen->w);
		return 1;
	}
	else if(strcmp(key, "math") == 0) {
		lua_getglobal(L, "math");
		return 1;
	}

	lua_getmetatable(L, -2);
	lua_pushvalue(L, -2);
	lua_rawget(L, -2);
	return 1;
}

static int
l_script_newindex(lua_State *L) {
	// object, key, value
	lua_getuservalue(L, -3);
	lua_pushvalue(L, -3);
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);

	return 0;
}

int
l_script_println(lua_State *L) {
	const char *message = luaL_checkstring(L, 1);
	printf("%s\n", message);
	return 0;
}

int
l_script_noop(lua_State *L) {
	return 0;
}

static struct luaL_Reg dblscriptlib[] = {
    {"init", l_script_init},
    {"__index", l_script_index},
    {"__newindex", l_script_newindex},
   	{"setup", l_script_noop},
	{"draw", l_script_noop},
	{"println", l_script_println},
    {NULL, NULL}
};