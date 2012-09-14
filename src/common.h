#ifndef __COMMON_H__
#define __COMMON_H__

#include <SDL/SDL.h>
#include "dabble.h"

typedef int ParamRef;

int param_checktype(Dabble *dbl, const char *paramname, int type);
ParamRef param_get(Dabble *dbl, const char *paramname);
int param_tointeger(Dabble *dbl, ParamRef ref);
const char *param_tostring(Dabble *dbl, ParamRef ref);
int param_listlength(Dabble *dbl, ParamRef ref);
ParamRef param_listget(Dabble *dbl, ParamRef ref, int index);
void param_callfunc(Dabble *dbl, ParamRef ref, const char *types, ...);

void dbl_noop(Dabble *dbl);

#define param_checkfunc(dbl, paramname)   param_checktype(dbl, paramname, LUA_TFUNCTION)
#define param_checknumber(dbl, paramname) param_checktype(dbl, paramname, LUA_TNUMBER)
#define param_checkstring(dbl, paramname) param_checktype(dbl, paramname, LUA_TSTRING)

#endif