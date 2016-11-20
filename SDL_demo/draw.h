#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"
#include "Matrix.h"

// Colours constants
const int	COLOUR_BLACK	= 0x00000000,
			COLOUR_WHITE	= 0x00FFFFFF,
			COLOUR_RED		= 0x00FF0000,
			COLOUR_GREEN	= 0x0000FF00,
			COLOUR_BLUE		= 0x000000FF,
			COLOUR_ORANGE	= 0x00FF9933;

const int	DRAWN_FIGURES_COUNT = 2, 
			HOW_MUCH_TO_DRAW_WITH_SECTION_WINDOW = 3, 
			RADIUS = 100;

// Used to easily switch between section window example and drawing multple figures
const bool	USE_SECTION_WINDOW = DRAWN_FIGURES_COUNT <= HOW_MUCH_TO_DRAW_WITH_SECTION_WINDOW;

// Used to compensate for possible mistakes in float type
const float	ALMOST_ZERO = 0.00000001;

void draw(SDL_Surface *s, Matrix mtrx_finals[DRAWN_FIGURES_COUNT], bool draw_inside);
void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 colour);
Uint32 get_pixel32(SDL_Surface *surface, int x, int y);

#endif