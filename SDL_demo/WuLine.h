#pragma once

#include "Line.h"
#include "stdlib.h"

class WuLine : public Line {
protected:
	void swap(float *first, float *second);
	void draw_wu_point(SDL_Surface *s, int x, int y, float intensity, bool more_vertical);
public:
	WuLine();
	WuLine(Colour colour);
	WuLine(const WuLine& copy);
	~WuLine();

	void draw(SDL_Surface* s, Point start, Point finish);

	WuLine& operator=(const WuLine& other);
};