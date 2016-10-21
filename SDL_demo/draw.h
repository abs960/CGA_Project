#ifndef DRAW_H
#define DRAW_H

#include "stdafx.h"

#define RAD(angle) ((angle) * M_PI / 180)
#define NEXT(cur, max) (((cur) + 1) % (max))
#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

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
const float	ALMOST_ZERO = 0.000001;

typedef float** Matrix;
struct Point {
	float x;
	float y;

	Point(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Point() : x(0), y(0) {}

	Point operator+(Point p) const {
		return Point(x + p.x, y + p.y);
	}

	Point operator-(Point p) const {
		return Point(x - p.x, y - p.y);
	}

	Point operator*(float m) const {
		return Point(m * x, m * y);
	}

	Point operator/(float d) const {
		return Point(x / d, y / d);
	}

	bool operator==(Point p) const {
		return x == p.x && y == p.y;
	}

	bool operator!=(Point p) const {
		return x != p.x || y != p.y;
	}
};
struct Line {
	Point start;
	Point finish;

	Line(Point start, Point finish) {
		this->start = start;
		this->finish = finish;
	}

	Line() : start(Point()), finish(Point()) {}
};

void draw(SDL_Surface *s, Matrix mtrx_finals[DRAWN_FIGURES_COUNT], bool draw_inside);
void mult_mtrx(Matrix left, Matrix right, Matrix result);
void mult_mtrx_vector(Matrix mtrx, float* vector, float* result);

#endif