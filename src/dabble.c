#include <stdlib.h>
#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "dabble.h"

static void *
dbl_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr == NULL) {
		fprintf(stderr, "Malloc failure. haha!");
		exit(EXIT_FAILURE);
	}
	return ptr;
}

Dabble *
new_dabble(SDL_Surface *screen) {
	lua_State *L = NULL;
	L = luaL_newstate();
	luaL_openlibs(L);

	// search and load the dabble lua library
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "searchpath");
	lua_pushstring(L, "dabble");
	lua_pushstring(L, DBL_LUA_PATH);
	lua_call(L, 2, 1);
	int retval = luaL_loadfile(L, lua_tostring(L, -1));
	if(retval != LUA_OK) {
		lua_close(L);
		fprintf(stderr, "Unable to load dabble lua library");
		return NULL;
	}
	lua_call(L, 0, 1);

	// set search path
	lua_pushstring(L, DBL_LUA_PATH);
	lua_setfield(L, -2, "path");
	int dbl_lib_ref = luaL_ref(L, LUA_REGISTRYINDEX);

	Dabble *dbl = NULL;
	dbl = dbl_malloc(sizeof(screen));
	dbl->screen = screen;
	dbl->L = L;
	dbl->dbl_lib_ref = dbl_lib_ref;
	return dbl;
}

void
free_dabble(Dabble *dbl) {
	lua_close(dbl->L);
	free(dbl);
}

int
do_script(Dabble *dbl, const char *name) {
	// call create function in the lua library
	lua_rawgeti(dbl->L, LUA_REGISTRYINDEX, dbl->dbl_lib_ref);
	lua_getfield(dbl->L, -1, "create");
	lua_pushvalue(dbl->L, -2);
	lua_pushstring(dbl->L, name);
	lua_remove(dbl->L, -4);
	lua_call(dbl->L, 2, 2);

	// check if there were errors
	if(!lua_isnil(dbl->L, -1)) {
		fprintf(stderr, "Unable to open dabble script %s: %s", name, lua_tostring(dbl->L, -1));
		return 0;
	}
	lua_pop(dbl->L, 1); // remove the nil error message

	// run setup
	lua_pushvalue(dbl->L, -1);
	lua_getfield(dbl->L, -1, "setup");
	lua_call(dbl->L, 0, 0);

	// bring draw function to stack top
	lua_getfield(dbl->L, -1, "draw");

	// main loop
	SDL_Event event;
	int quit = 0;
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = 1;
			}
		}
		lua_pushvalue(dbl->L, -1);
		lua_call(dbl->L, 0, 0);
	}

	lua_pop(dbl->L, 1);
	return 1;
}
