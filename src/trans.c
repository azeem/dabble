#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "trans.h"

Dabble*
movement_init(const char *type_name, SDL_Surface *screen, lua_State *L) {
	DabbleMovement *dbl_mov = malloc(sizeof(DabbleMovement));
	memset(dbl_mov, 0, sizeof(DabbleMovement));
	dbl_mov->dbl.screen = screen;
	dbl_mov->dbl.L = L;
	dbl_mov->dbl.type = &dbl_movementtype;
	return (Dabble*)dbl_mov;
}

void
movement_noop(Dabble *dbl) {
	fprintf(stderr, "NOOP\n");
	return;
}

DabbleType dbl_movementtype = {
	"Movement",
	movement_init,
	movement_noop,
	movement_noop
};