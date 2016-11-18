#pragma once

#include "Point.h"
#include "SDL.h"
#include "Colour.h"

class Line {
private:
	Point start;
	Point finish;
	Colour colour;
public:
	Line();
	Line(Point start, Point finish);
	Line(const Line& copy);
	~Line();

	Point get_start();
	Point get_finish();
	Colour get_colour();
	void set_colour(Colour colour);

	virtual void draw(SDL_Surface *s) =0;
};