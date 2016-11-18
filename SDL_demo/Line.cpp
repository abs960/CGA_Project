#include "Line.h"

Line::Line() : Line(Point(), Point()) {}

Line::Line(Point start, Point finish) {
	this->start = start;
	this->finish = finish;
}

Line::Line(const Line & copy) {
	start = copy.start;
	finish = copy.finish;
}

Line::~Line() {
}

Point Line::get_start() {
	return start;
}

Point Line::get_finish() {
	return finish;
}

Colour Line::get_colour() {
	return colour;
}

void Line::set_colour(Colour colour) {
	this->colour = colour;
}
