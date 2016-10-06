#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"

#define RAD(angle) ((angle) * M_PI / 180)

const int	DRAWN_FIGURES_COUNT = 2, 
			RADIUS = 100;

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