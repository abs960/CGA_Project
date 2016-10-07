#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"

#define RAD(angle) ((angle) * M_PI / 180)
#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

const int	DRAWN_FIGURES_COUNT = 4, 
			RADIUS = 100, 
			HOW_MUCH_YOU_REALLY_WANT_TO_DRAW = 2;

const bool	USE_SECTION_WINDOW = DRAWN_FIGURES_COUNT <= HOW_MUCH_YOU_REALLY_WANT_TO_DRAW;

typedef float** Matrix;
struct Point {
	float x;
	float y;
};
struct Line {
	Point start;
	Point finish;
	float x_coef;
	float y_coef;
	float no_coef;
};

void draw(SDL_Surface *s, Matrix mtrxF[DRAWN_FIGURES_COUNT], int figure_count, bool draw_inside);
void mult_mtrx(Matrix left, Matrix right, Matrix result);
void mult_mtrx_vector(Matrix mtrx, float* vector, float* result);

#endif