#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "dabble.h"
#include "trans.h"
#include "script.h"

#define debug(...) { fprintf(stderr, "DEBUG:" __VA_ARGS__); fputc('\n', stderr);}

DabbleType *dbl_typelist[] = {
	&dbl_movementtype,
	NULL
};

Dabble*
load_dabble(lua_State *L, const char *dbl_typename, SDL_Surface *screen) {
	// search for dabble type in C dabbles list
	DabbleType **list_item = dbl_typelist;
	DabbleType *dbl_type;
	while((*list_item) != NULL) {
		dbl_type = *list_item;
		if(strcmp(dbl_type->name, dbl_typename) == 0) {
			break;
		}
		list_item++;
	}
	if((*list_item) == NULL) {
		dbl_type = &dbl_scripttype;
	}

	Dabble *dbl = (Dabble*)malloc(dbl_type->size);
	memset(dbl, 0, sizeof(dbl_type->size));
	dbl->screen = screen;
	dbl->L = L;
	dbl->type = dbl_type;
	dbl->param = luaL_ref(L, LUA_REGISTRYINDEX);
	
	if(dbl_type->init(dbl, dbl_typename)) {
		return dbl;
	}
	else {
		free(dbl);
		return NULL;
	}
}

void
run_dabble(Dabble *dbl) {
	debug("Runnin setup");
	dbl->type->setup(dbl);

	debug("Begin Main Loop");
	// main loops
	SDL_Event event;
	int quit = 0;
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = 1;
			}
		}
		dbl->type->draw(dbl);
		SDL_UpdateRect(dbl->screen, 0, 0, dbl->screen->w, dbl->screen->h); 
	}
}