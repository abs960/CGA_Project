#include "BrezenheimLine.h"

BrezenheimLine::BrezenheimLine() : BrezenheimLine(Colour()) {}

BrezenheimLine::BrezenheimLine(Colour colour) {
	this->colour = colour;
}

BrezenheimLine::BrezenheimLine(const BrezenheimLine & copy) {
	colour = copy.colour;
}

BrezenheimLine::~BrezenheimLine() {}

void BrezenheimLine::draw(SDL_Surface * s, Point start, Point finish) {
	int delta_x = abs(finish.x - start.x);
	int delta_y = abs(finish.y - start.y);
	int step_x = finish.x >= start.x ? 1 : -1;
	int step_y = finish.y >= start.y ? 1 : -1;
	if (delta_y <= delta_x) {
		int d = (delta_y << 1) - delta_x;
		int d1 = delta_y << 1;
		int d2 = (delta_y - delta_x) << 1;
		put_pixel32(s, start.x, start.y, colour.get_value());
		for (int x = start.x + step_x, y = start.y, i = 1; i < delta_x; i++, x += step_x) {
			if (d > 0) {
				d += d2;
				y += step_y;
			} else
				d += d1;
			put_pixel32(s, x, y, colour.get_value());
		}
	} else {
		int d = (delta_x << 1) - delta_y;
		int d1 = delta_x << 1;
		int d2 = (delta_x - delta_y) << 1;
		put_pixel32(s, start.x, start.y, colour.get_value());
		for (int x = start.x, y = start.y + step_y, i = 1; i <= delta_y; i++, y += step_y) {
			if (d > 0) {
				d += d2;
				x += step_x;
			} else
				d += d1;
			put_pixel32(s, x, y, colour.get_value());
		}
	}
}

BrezenheimLine & BrezenheimLine::operator=(const BrezenheimLine & other) {
	if (&other == this)
		return (*this);

	colour = other.colour;

	return (*this);
}
