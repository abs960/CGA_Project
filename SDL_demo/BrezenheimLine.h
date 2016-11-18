#pragma once

#include "Line.h"
#include "stdlib.h"

class BrezenheimLine : public Line {
public:
	BrezenheimLine();
	BrezenheimLine(Colour colour);
	BrezenheimLine(const BrezenheimLine& copy);
	~BrezenheimLine();

	void draw(SDL_Surface* s, Point start, Point finish);

	BrezenheimLine& operator=(const BrezenheimLine& other);
};

