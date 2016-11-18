#include "WuLine.h"

WuLine::WuLine() : WuLine(Colour()) {}

WuLine::WuLine(Colour colour) {
	this->colour = colour;
}

WuLine::WuLine(const WuLine & copy) {
	colour = copy.colour;
}

WuLine::~WuLine() {}

void WuLine::draw(SDL_Surface * s, Point start, Point finish) {
	bool more_vertical = abs(finish.y - start.y) > abs(finish.x -start.x);
	if (more_vertical) {
		swap(&start.x, &start.y);
		swap(&finish.x, &finish.y);
	}
	if (start.x > finish.x) {
		swap(&start.x, &finish.x);
		swap(&start.y, &finish.y);
	}

	draw_wu_point(s, start.x, start.y, 1, more_vertical);
	draw_wu_point(s, finish.x, finish.y, 1, more_vertical);
	float dx = finish.x - start.x;
	float dy = finish.y - start.y;
	float k = dy / dx;
	float y = start.y + k;
	for (int x = start.x + 1; x <= finish.x - 1; x++) {
		int y_int = floor(y);
		float intensity = y - y_int;
		draw_wu_point(s, x, y_int, 1 - intensity, more_vertical);
		draw_wu_point(s, x, y_int + 1, intensity, more_vertical);
		y += k;
	}
}

WuLine & WuLine::operator=(const WuLine & other) {
	if (&other == this)
		return (*this);

	colour = other.colour;

	return (*this);
}

void WuLine::swap(float *first, float *second) {
	float tmp = *first;
	*first = *second;
	*second = tmp;
}

void WuLine::draw_wu_point(SDL_Surface * s, int x, int y, float intensity, bool more_vertical) {
	Colour new_colour = colour;
	new_colour.change_intensity(intensity);
	if (more_vertical)
		put_pixel32(s, y, x, new_colour.get_value());
	else
		put_pixel32(s, x, y, new_colour.get_value());
}