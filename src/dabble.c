#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "dabble.h"

#define debug(...) { fprintf(stderr, "DEBUG:" __VA_ARGS__); fputc('\n', stderr);}

void
load_dabble(lua_State *L, const char *script_name, SDL_Surface *screen) {
	debug("Loading dabble script");
	luaL_getmetatable(L, "DabbleScript");
	lua_getfield(L, -1, "init");
	lua_pushstring(L, script_name);
	lua_pushlightuserdata(L, screen);
	lua_call(L, 2, 1);
}

void
run_dabble(lua_State *L) {
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