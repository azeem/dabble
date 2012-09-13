#ifndef __COMMON_H__
#define __COMMON_H__

#include <SDL/SDL.h>
#include "dabble.h"

int param_checktype(Dabble *dbl, const char *paramname, int reqtype);
void param_callfunc(Dabble *dbl, const char *paramname, const char *types, ...);

void dbl_noop(Dabble *dbl);

#define param_checkfunc(dbl, paramname)   param_checktype(dbl, paramname, LUA_TFUNCTION)
#define param_checknumber(dbl, paramname) param_checktype(dbl, paramname, LUA_TNUMBER)
#define param_checkstring(dbl, paramname) param_checktype(dbl, paramname, LUA_TSTRING)

#endif