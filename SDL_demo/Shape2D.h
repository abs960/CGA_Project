#pragma once

#include "Colour.h"
#include "Line.h"

#define DEFAULT_SIDE_COUNT 4
#define DEFAULT_RADIUS 100

class Shape2D {
protected:
	Colour colour;
	Line* brush;
	Point* points;

	int radius;
	int side_count;

	void init_points();
public:
	Shape2D();
	Shape2D(int side_count);
	Shape2D(int side_count, int radius);
	Shape2D(const Shape2D& copy);
	~Shape2D();

	void set_colour(Colour colour);
	Colour get_colour();
	void set_side_count(int side_count);
	int get_side_count();
	void set_radius(int radius);
	int get_radius();
	void set_brush(Line* brush);

	void draw(SDL_Surface* s);

	Shape2D& operator=(const Shape2D& other);
};

