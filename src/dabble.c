#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL_gfxPrimitives.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <time.h>
#include "dabble.h"

#define debug(...) { fprintf(stderr, "DEBUG:" __VA_ARGS__); fputc('\n', stderr);}

/*static void *
dbl_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr == NULL) {
		fprintf(stderr, "Malloc failure. haha!");
		exit(EXIT_FAILURE);
	}
	return ptr;
}*/

static struct luaL_Reg dabblelib[];
static struct dbl_const_item {
	const char *name;
	int value;
} dabbleconstants[]  = {
	{"CENTER",  CENTER},
	{"RADIUS",  RADIUS},
	{"CORNER",  CORNER},
	{"CORNERS", CORNERS},
	{NULL, 0},
};

void
dbl_init_lua(lua_State *L) {
	debug("Initializing lua State");
	// load dabble class
	luaL_newmetatable(L, "Dabble");
	luaL_setfuncs(L, dabblelib, 0);
	// intialize the constants
	struct dbl_const_item *const_item = dabbleconstants;
	while(const_item->name != NULL) {
		lua_pushvalue(L, -1);
		lua_pushinteger(L, const_item->value);
		lua_setfield(L, -2, const_item->name);
		const_item++;
	}

	// init dbl_loaded table that stores closures of loaded scripts
	lua_newtable(L);
	lua_setglobal(L, "dbl_loaded");

	// seed random
	lua_getglobal(L, "math");
	lua_getfield(L, -1, "randomseed");
	lua_pushinteger(L, time(NULL));
	lua_call(L, 1, 0);
}

void
dbl_load_dabble(lua_State *L, const char *script_name) {
	debug("Loading dabble script");
	luaL_getmetatable(L, "Dabble");
	lua_getfield(L, -1, "new");
	lua_pushstring(L, script_name);
	lua_call(L, 1, 1);
}

void
dbl_run_dabble(lua_State *L) {
	// stack: dabble_object
	Dabble *dbl = (Dabble*)lua_touserdata(L, -1);

	debug("Running setup");
	// run setup
	lua_pushvalue(L, -1);
	lua_getfield(L, -1, "setup");
	lua_call(L, 0, 0);

	lua_pushvalue(L, -1);
	lua_getfield(L, -1, "draw");

	debug("Begin Main Loop");
	// main loop
	SDL_Event event;
	int quit = 0;
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = 1;
			}
		}
		lua_pushvalue(L, -1);
		lua_call(L, 0, 0);
		SDL_UpdateRect(dbl->screen, 0, 0, dbl->screen->w, dbl->screen->h); 
	}
}

/* Begin Dabble Lua class functions */

static int
dbl_index(lua_State *L) {
	// object, key
	lua_getuservalue(L, -2);
	lua_pushvalue(L, -2);
	lua_rawget(L, -2);
	if(!lua_isnoneornil(L, -1)) {
		return 1;
	}

	lua_pop(L, 2);

	const char *key = lua_tostring(L, -1);
	const Dabble *dbl = (Dabble *)lua_touserdata(L, -2);
	if(strcmp(key, "height") == 0) {
		lua_pushinteger(L, dbl->screen->h);
		return 1;
	}
	else if(strcmp(key, "width") == 0) {
		lua_pushinteger(L, dbl->screen->w);
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
dbl_new_index(lua_State *L) {
	// object, key, value
	lua_getuservalue(L, -3);
	lua_pushvalue(L, -3);
	lua_pushvalue(L, -3);
	lua_rawset(L, -3);

	return 0;
}

static int
dbl_new(lua_State *L) {
	debug("Creating new dabble object");
	// stack: script_name
	const char *script_name = luaL_checkstring(L, 1);

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
	Dabble *dbl = lua_newuserdata(L, sizeof(Dabble));
	memset(dbl, 0, sizeof(Dabble));
	luaL_setmetatable(L, "Dabble");
	lua_newtable(L);
	lua_setuservalue(L, -2);

	// set _ENV of script and run it
	//stack: object, script_function
	lua_pushvalue(L, -1);
	lua_setupvalue(L, -3, 1);
	lua_pushvalue(L, -2);
	lua_call(L, 0, 0);
	return 1;
}

int
dbl_println(lua_State *L) {
	const char *message = luaL_checkstring(L, 1);
	printf("%s\n", message);
	return 0;
}



int
dbl_noop(lua_State *L) {
	return 0;
}

static struct luaL_Reg dabblelib[] = {
	{"setup", dbl_noop},
	{"draw", dbl_noop},

    {"new", dbl_new},
    {"__index", dbl_index},
    {"__newindex", dbl_new_index},
    {"println", dbl_println},
    {NULL, NULL}
};