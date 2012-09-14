#ifndef __DABBLE_H__
#define __DABBLE_H__

#include <SDL/SDL.h>
#include <lua.h>

struct Dabbletype;

typedef struct Dabble {
	SDL_Surface *screen;
	lua_State *L;
	struct DabbleType *type;
	int param;
} Dabble;

typedef struct DabbleType {
	const char *name;
	int  (*init)(Dabble *dbl, const char *dbl_typename);
	void (*setup)(Dabble *dbl);
	void (*draw)(Dabble *dbl);
	void (*destroy)(Dabble *dbl);
	size_t size;
} DabbleType;

Dabble *load_dabble(lua_State *L, const char *dbl_typename, SDL_Surface *screen);
void run_dabble(Dabble *dbl);
#endif