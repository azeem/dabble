#ifndef __DABBLE_H__
#define __DABBLE_H__

#include <SDL/SDL.h>
#include <lua.h>

struct Dabbletype;

typedef struct Dabble {
	SDL_Surface *screen;
	lua_State *L;
	struct DabbleType *type;
	int self;
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

#define DBL(dbl) ((Dabble*)dbl)

#define DBL_WEAKREF_INDEX	"68491445-6a8c-4958-8de1-de7cfe76f9ad"
#define DBL_STRONGREF_INDEX  "ea401fa2-f1a1-4040-b363-923931b8609f"

void open_dbllib(lua_State *L);
Dabble* new_dabble(lua_State *L, const char *dbl_typename, SDL_Surface *screen);
void free_dabble(Dabble *);
void run_dabble(Dabble *dbl);

#endif