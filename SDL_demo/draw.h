#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"

#define RAD(angle) ((angle) * M_PI / 180)

typedef float** Matrix;
struct Point {
	float x;
	float y;
};

void draw(SDL_Surface *s, Matrix mtrxF, int side_count, int figure_count);
void mult_mtrx(Matrix left, Matrix right, Matrix result);
void mult_mtrx_vector(Matrix mtrx, float* vector, float* result);

#endif