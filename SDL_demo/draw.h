#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 colour);
Uint32 get_pixel32(SDL_Surface *surface, int x, int y);

#endif