#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "dabble.h"
#include "trans.h"
#include "script.h"

#define debug(...) { fprintf(stderr, "DEBUG:" __VA_ARGS__); fputc('\n', stderr);}

static luaL_Reg dbllib[];

void
open_dbllib(lua_State *L) {
	lua_newmetatable(L, "Dabble");
	luaL_setfuncs(L, dbllib, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
}

static DabbleType *dbl_typelist[] = {
	&dbl_movementtype,
	NULL
};

Dabble*
load_dabble(lua_State *L, const char *dbl_typename, SDL_Surface *screen, int luaobj) {
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

	Dabble *obj = NULL;
	if(luaobj) {
		dbl = (Dabble*)lua_newuserdata(dbl_type->size);
		luaL_setmetatable(L, "Dabble");
		lua_pop(L, 1);
	}
	else {
		dbl = (Dabble*)malloc(dbl_type->size);
	}
	memset(dbl, 0, sizeof(dbl_type->size));
	dbl->screen = screen;
	dbl->L = L;
	dbl->type = dbl_type;
	dbl->param = luaL_ref(L, LUA_REGISTRYINDEX);
	
	if(dbl_type->init(dbl, dbl_typename)) {
		return dbl;
	}
	else {
		if(!luaobj) {
			free(dbl);
		}
		return NULL;
	}
}

void
destroy_dabble(Dabble *dbl) {
	dbl->type->destroy(dbl);
	luaL_unref(dbl->L, LUA_REGISTRYINDEX, dbl->param);
	free(dbl);
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

/* Begin Dabble lua class functions */

int
l_dabble_setup(lua_State *L) {
	Dabble *dbl = (Dabble *)lua_checkudata(L, 1, "Dabble");
	dbl->type->setup(dbl);
	return 0;
}

int
l_dabble_draw(lua_State *L) {
	Dabble *dbl = (Dabble *)lua_checkudata(L, 1, "Dabble");
	dbl->type->draw(dbl);
}

int
l_dabble_gc(lua_State *L) {
	Dabble *dbl = (Dabbe *)lua_touserdata(L, -1);
	dbl->type->destroy(dbl);
	luaL_unref(L, LUA_REGISTRYINDEX, dbl->param);
	return 0;
}

static luaL_Reg dbllib[] = {
	{"__gc", l_dabble_gc},
	{"setup", l_dabble_setup},
	{"draw", l_dabble_draw},
	{NULL, NULL}
};