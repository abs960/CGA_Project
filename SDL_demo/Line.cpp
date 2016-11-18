#include "Line.h"

Line::Line() : Line(Colour()) {}

Line::Line(Colour colour) {
	this->colour = colour;
}

Line::Line(const Line & copy) {
	colour = copy.colour;
}

Line::~Line() {}

Colour Line::get_colour() {
	return colour;
}

void Line::set_colour(Colour colour) {
	this->colour = colour;
}
