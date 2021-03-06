#pragma once

#include "Colour.h"
#include "Line.h"

class Shape2D {
protected:
	static const int DEFAULT_SIDE_COUNT = 4;
	static const int DEFAULT_RADIUS = 100;

	Colour colour;
	Line* brush;
	Point* points;

	int radius;
	int side_count;

	virtual void init_points();
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

	virtual void draw(SDL_Surface* s);
	Point get_point(int index);

	Shape2D& operator=(const Shape2D& other);
};

