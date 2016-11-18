#pragma once

#include "Point.h"
#include "SDL.h"
#include "Colour.h"
#include "draw.h"

class Line {
protected:
	Colour colour;
public:
	Line();
	Line(Colour colour);
	Line(const Line& copy);
	~Line();

	Colour get_colour();
	void set_colour(Colour colour);

	virtual void draw(SDL_Surface *s, Point start, Point finish) =0;
};