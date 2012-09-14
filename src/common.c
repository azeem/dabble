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
	if(lua_isnil(dbl->L, -1)) {
		return 0;
	}
	lua_getfield(dbl->L, -1, paramname);
	int type = lua_type(dbl->L, -1);
	lua_pop(dbl->L, 2);
	return (reqtype == type);
}

ParamRef
param_get(Dabble *dbl, const char *paramname) {
	lua_rawgeti(dbl->L, LUA_REGISTRYINDEX, dbl->param);
	lua_getfield(dbl->L, -1, paramname);
	lua_remove(dbl->L, -2);
	return lua_gettop(dbl->L);
}

// C data conversion functions
int
param_tointeger(Dabble *dbl, ParamRef ref) {
	return lua_tointeger(dbl->L, ref);
}

const char*
param_tostring(Dabble *dbl, ParamRef ref) {
	return lua_tostring(dbl->L, ref);
}

// list operators
int
param_listlength(Dabble *dbl, ParamRef ref) {
	lua_len(dbl->L, ref);
	int len = lua_tointeger(dbl->L, -1);
	lua_pop(dbl->L, 1);
	return len;
}

ParamRef
param_listget(Dabble *dbl, ParamRef ref, int index) {
	lua_rawgeti(dbl->L, ref, index-1);
	return lua_gettop(dbl->L);
}

// function operators
void
param_callfunc(Dabble *dbl, ParamRef ref, const char *types, ...) {
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
	lua_pushvalue(L, ref);

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