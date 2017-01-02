#include "draw.h"

double screenX(double x) {
	return SCREEN_WIDTH / 2 + x;
}

double screenY(double y) {
	return SCREEN_HEIGHT / 2 + y;
}

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 colour) {
	assert(NULL != surface);

	x = screenX(x);
	y = screenY(y);

	if (x < 0 || x > SCREEN_WIDTH ||
		y < 0 || y > SCREEN_HEIGHT)
		return;

	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = colour;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y) {
	assert(NULL != surface);

	x = screenX(x);
	y = screenY(y);

	if (x < 0 || x > SCREEN_WIDTH ||
		y < 0 || y > SCREEN_HEIGHT)
		return -1;

	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}
