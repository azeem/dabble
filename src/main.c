#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "dabble.h"

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480
#define DEFAULT_SCREEN_BPP 32
#define DEFAULT_WINDOW_TITLE "Dabble"

SDL_Surface *screen = NULL;
Dabble *dbl = NULL;

void cleanup();

int
init() {
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

	dbl = new_dabble(screen);
	if(!dbl) {
		cleanup();
		return 0;
	}
	return 1;
}

void cleanup() {
	if(dbl) {
		free_dabble(dbl);
	}
	if(screen) {
		SDL_FreeSurface(screen);
		SDL_Quit();
	}
}

int
main(int argc, char **argv) {
	const char *script_name = "test";
	if(!init()) {
		return EXIT_FAILURE;
	}
	do_script(dbl, script_name);
	printf("Finished");
	cleanup();
	return EXIT_SUCCESS;
}
