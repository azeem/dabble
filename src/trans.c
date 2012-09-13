#include <SDL/SDL.h>
#include "trans.h"
#include "common.h"

int
movement_init(Dabble *dbl, const char *type_name) {
	SDL_Surface *screen = dbl->screen;
	DBLMOV(dbl)->buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,
											   screen->w, screen->h,
											   screen->format->BitsPerPixel, 
                    	              		   screen->format->Rmask,
                        	          		   screen->format->Gmask,
                            	      		   screen->format->Bmask,
                                	  		   screen->format->Amask);
	if(!DBLMOV(dbl)->buffer) {
		fprintf(stderr, "Unable to create SDL Buffer");
		return 0;
	}
	if(!param_checkfunc(dbl, "func")) {
		fprintf(stderr, "'func' param is required and should be a function");
		return 0;	
	}
	return 1;
}

void
movement_draw(Dabble *dbl) {
	DabbleMovement *dbl_mov = (DabbleMovement*)dbl;
	SDL_Surface *buffer = dbl_mov->buffer;
	SDL_Surface *screen = dbl->screen;

	SDL_FillRect(buffer, NULL, 0);
	int x, y, nx, ny;
    int bpp = screen->format->BytesPerPixel;
    Uint8 *src, *dest;
	for(x = 0;x < buffer->w;x++) {
		for(y = 0;y < buffer->h;y++) {
			param_callfunc(dbl, "func", "II:II", x, y, &nx, &ny);
			src  = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
			dest = (Uint8 *)buffer->pixels + ny * buffer->pitch + nx * bpp;
			memcpy(dest, src, bpp);
		}
	}
	SDL_BlitSurface(buffer, NULL, screen, NULL);
}

DabbleType dbl_movementtype = {
	"Movement",
	movement_init,
	dbl_noop,
	movement_draw,
	sizeof(DabbleMovement)
};