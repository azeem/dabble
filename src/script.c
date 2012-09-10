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
	luaL_newmetatable(L, "DabbleScriptEnv");
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


Dabble *dblscript_init(const char *type_name, SDL_Surface *screen, lua_State *L) {
	int stack_top = lua_gettop(L);

	lua_getglobal(L, "dbl_loaded");
	lua_getfield(L, -1, type_name);
	if(lua_isnil(L, -1)) {
		// search the script in the path
		lua_getglobal(L, "package");
		lua_getfield(L, -1, "searchpath");
		lua_pushstring(L, type_name);
		lua_pushstring(L, DBL_LUA_PATH);
		lua_call(L, 2, 1);
		if(lua_isnil(L, -1)) {
			fprintf(stderr, "Dabble script '%s' not found", type_name);
			lua_settop(L, stack_top);
			return NULL;
		}

		// load the script
		int load_status = luaL_loadfile(L, lua_tostring(L, -1));
		if(load_status != LUA_OK) {
			fprintf(stderr, "Dabble script '%s' could not be loaded", type_name);
			lua_settop(L, stack_top);
			return NULL;
		}

		// stack: script_function, script_path, nil, dbl_loaded
		lua_pushvalue(L, -1);
		lua_setfield(L, -4, type_name);
	}

	// create a new dabble object
	DabbleScript *dbl_script = malloc(sizeof(DabbleScript));
	memset(dbl_script, 0, sizeof(DabbleScript));
	dbl_script->dbl.screen = screen;
	dbl_script->dbl.L = L;
	dbl_script->dbl.type = &dbl_scripttype;

	// setup the script environment
	lua_newtable(L);
	new_canvas(L, screen);
	lua_setfield(L, -2, "canvas");
	lua_pushlightuserdata(L, dbl_script);
	lua_setfield(L, -2, "dbl_script");
	lua_pushvalue(L, -1);
	luaL_getmetatable(L, "DabbleScriptEnv");
	lua_setmetatable(L, -2);
	dbl_script->env = luaL_ref(L, LUA_REGISTRYINDEX);

	// set _ENV of script and run it
	//stack: object, script_function
	lua_pushvalue(L, -1);
	lua_setupvalue(L, -3, 1);
	lua_pushvalue(L, -2);
	lua_call(L, 0, 0);

	lua_settop(L, stack_top);
	return (Dabble*)dbl_script;
}

static void
call_envfunc(const char *funcname, Dabble *dbl) {
	lua_State *L = dbl->L;
	int stack_top = lua_gettop(L);
	DabbleScript *dbl_script = (DabbleScript*)dbl;
	lua_rawgeti(L, LUA_REGISTRYINDEX, dbl_script->env);
	lua_getfield(L, -1, funcname);
	if(!lua_isnil(L, -1)) {
		lua_call(L, 0, 0);
	}
	lua_settop(L, stack_top);

}

void dblscript_setup(Dabble *dbl) {
	call_envfunc("setup", dbl);
}

void dblscript_draw(Dabble *dbl) {
	call_envfunc("draw", dbl);	
}

struct DabbleType dbl_scripttype = {
	"DabbleScript",
	dblscript_init,
	dblscript_setup,
	dblscript_draw
};

/* Begin Dabble Lua class functions */

static int
l_script_index(lua_State *L) {
	// object, key
	lua_pushvalue(L, -1);
	lua_rawget(L, -3);
	if(!lua_isnoneornil(L, -1)) {
		return 1;
	}

	lua_pop(L, 1);

	const char *key = lua_tostring(L, -1);
	lua_getfield(L, -2, "dbl_script");
	const DabbleScript *dbl_script = (DabbleScript *)lua_touserdata(L, -1);//lua_touserdata(Lua, -2);
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

	lua_pop(L, 1);

	lua_getmetatable(L, -2);
	lua_pushvalue(L, -2);
	lua_rawget(L, -2);
	return 1;
}

int
l_script_println(lua_State *L) {
	const char *message = luaL_checkstring(L, 1);
	printf("%s\n", message);
	return 0;
}

static struct luaL_Reg dblscriptlib[] = {
    {"__index", l_script_index},
    //{"__newindex", l_script_newindex},
	{"println", l_script_println},
    {NULL, NULL}
};