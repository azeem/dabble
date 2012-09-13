#include <ctype.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "dabble.h"
#include "common.h"

int
param_checktype(Dabble *dbl, const char *paramname, int reqtype) {
	lua_rawgeti(dbl->L, LUA_REGISTRYINDEX, dbl->param);
	lua_getfield(dbl->L, -1, paramname);
	int type = lua_type(dbl->L, -1);
	lua_pop(dbl->L, 2);
	return (type == reqtype);
}

void
param_callfunc(Dabble *dbl, const char *paramname, const char *types, ...) {
	lua_State *L = dbl->L;
	int stacktop = lua_gettop(L);

	char *typestr = strdup(types);
	char *argtypes = typestr;
	char *restypes = typestr;
	while(*restypes != ':') restypes++;
	*restypes = '\0';
	restypes++;
	int nargs = strlen(argtypes);
	int nresults = strlen(restypes);

	va_list argp;
	va_start(argp, types);

	// push the function
	lua_rawgeti(L, LUA_REGISTRYINDEX, dbl->param);
	lua_getfield(L, -1, paramname);

	// push the arguments
	while(*argtypes) {
		switch(toupper(*argtypes)) {
			case 'I':
				lua_pushinteger(L, va_arg(argp, int));
				break;
			case 'S':
				lua_pushstring(L, va_arg(argp, char*));
				break;
		}
		argtypes++;
	}

	lua_call(L, nargs, nresults);
	
	// pop the results
	int resindex = -nresults;
	while(toupper(*restypes)) {
		switch(toupper(*restypes)) {
			case 'I':
				*(va_arg(argp, int*)) = lua_tointeger(L, resindex);
				break;
			case 'S':
				*(va_arg(argp, const char**)) = lua_tostring(L, resindex);
				break;
		}
		resindex++;
		restypes++;
	}

	free(typestr);
	lua_settop(L, stacktop);
}

void
dbl_noop(Dabble *dbl) {
	return;
}