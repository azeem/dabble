#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "dabble.h"
#include "script.h"
#include "canvas.h"
#include "trans.h"

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480
#define DEFAULT_SCREEN_BPP 32
#define DEFAULT_WINDOW_TITLE "Dabble"

static SDL_Surface *screen = NULL;
static lua_State *L = NULL;

void cleanup();

int
init() {
	// initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return 0;
	}
	screen = SDL_SetVideoMode(DEFAULT_SCREEN_WIDTH,
							  DEFAULT_SCREEN_HEIGHT,
							  DEFAULT_SCREEN_BPP,
							  SDL_SWSURFACE);
	if(!screen) {
		return 0;
	}
	SDL_WM_SetCaption(DEFAULT_WINDOW_TITLE, NULL);

	// intialize lua
	L = luaL_newstate();
	luaL_openlibs(L);
	open_canvaslib(L);
	open_dblscriptlib(L);
	open_dbllib(L);
	return 1;
}

void cleanup() {
	if(screen) {
		SDL_FreeSurface(screen);
		SDL_Quit();
	}
}

int
main(int argc, char **argv) {
	const char *script_name = "trans_test";
	if(!init()) {
		return EXIT_FAILURE;
	}

	Dabble *dbl = new_dabble(L, script_name, screen);
	if(!dbl) {
		return 1;
	}
	run_dabble(dbl);
	free_dabble(dbl);
	
	cleanup();
	return EXIT_SUCCESS;
}
