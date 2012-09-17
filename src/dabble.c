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
	luaL_newmetatable(L, "Dabble");
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
create_dabble(lua_State *L, const char *dbl_typename, SDL_Surface *screen) {
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

	Dabble *dbl = (Dabble*)lua_newuserdata(L, dbl_type->size);
	luaL_setmetatable(L, "Dabble");
	memset(dbl, 0, sizeof(dbl_type->size));
	dbl->screen = screen;
	dbl->L = L;
	dbl->type = dbl_type;
	dbl->param = luaL_ref(L, LUA_REGISTRYINDEX);
	
	if(dbl_type->init(dbl, dbl_typename)) {
		return dbl;
	}
	else {
		lua_pop(L, 1);
		return NULL;
	}
}

void
run_dabble(Dabble *dbl) {
	int stacktop;
	stacktop = lua_gettop(dbl->L);
	debug("Runnin setup");
	dbl->type->setup(dbl);
	lua_settop(dbl->L, stacktop);

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

		stacktop = lua_gettop(dbl->L);
		dbl->type->draw(dbl);
		lua_settop(dbl->L, stacktop);
		SDL_UpdateRect(dbl->screen, 0, 0, dbl->screen->w, dbl->screen->h); 
	}
}

/* Begin Dabble lua class functions */

int
l_dabble_setup(lua_State *L) {
	Dabble *dbl = DBL(luaL_checkudata(L, 1, "Dabble"));
	dbl->type->setup(dbl);
	return 0;
}

int
l_dabble_draw(lua_State *L) {
	Dabble *dbl = DBL(luaL_checkudata(L, 1, "Dabble"));
	dbl->type->draw(dbl);
	return 0;
}

int
l_dabble_gc(lua_State *L) {
	Dabble *dbl = (Dabble *)lua_touserdata(L, -1);
	dbl->type->destroy(dbl);
	luaL_unref(L, LUA_REGISTRYINDEX, dbl->param);
	return 0;
}

int
l_dabble_create(lua_State *L) {
	Dabble *dbl = DBL(luaL_checkudata(L, 1, "Dabble"));
	luaL_checktype(L, 2, LUA_TTABLE);
	
	lua_getfield(L, -1, "type");
	const char *typename = lua_tostring(L, -1);
	lua_pushvalue(L, -2);
	create_dabble(L, typename, dbl->screen);
	return 1;
}

static luaL_Reg dbllib[] = {
	{"__gc", l_dabble_gc},
	{"setup", l_dabble_setup},
	{"draw", l_dabble_draw},
	{"create", l_create_draw},
	{NULL, NULL}
};