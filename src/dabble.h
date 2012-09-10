#ifndef __DABBLE_H__
#define __DABBLE_H__

#include <SDL/SDL.h>
#include <lua.h>

struct Dabbletype;

typedef struct Dabble {
	SDL_Surface *screen;
	lua_State *L;
	struct DabbleType *type;
} Dabble;

typedef struct DabbleType {
	const char *name;
	Dabble *(*init)(const char *type_name, SDL_Surface *screen, lua_State *L);
	void (*setup)(Dabble *dbl);
	void (*draw)(Dabble *dbl);
} DabbleType;

Dabble *load_dabble(lua_State *L, const char *script_name, SDL_Surface *screen);
void run_dabble(Dabble *dbl);

#endif