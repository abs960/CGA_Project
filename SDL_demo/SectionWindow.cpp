#include "SectionWindow.h"

SectionWindow::SectionWindow() : SectionWindow(DEFAULT_SIDE_COUNT, DEFAULT_RADIUS) {}

SectionWindow::SectionWindow(int side_count) : SectionWindow(side_count, DEFAULT_RADIUS) {}

SectionWindow::SectionWindow(int side_count, int radius) {
	matrix = new Matrix(3);
	colour = Colour();
	brush = nullptr;

	transparent = true;
	this->side_count = side_count;
	this->radius = radius;
	init_points();

	center = Point();
	angle = 0.0f;
	scale = 0.0f;
}

SectionWindow::SectionWindow(const SectionWindow & copy) {
	*matrix = *(copy.matrix);
	colour = copy.colour;
	*brush = *(copy.brush);

	transparent = copy.transparent;
	side_count = copy.side_count;
	radius = copy.radius;
	center = copy.center;

	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = copy.points[i];
}

SectionWindow::~SectionWindow() {}

void SectionWindow::set_transparent(bool transparent) {
	this->transparent = transparent;
}

bool SectionWindow::is_transparent() {
	return transparent;
}

SectionWindow & SectionWindow::operator=(const SectionWindow & other) {
	if (&other == this)
		return (*this);

	*matrix = *(other.matrix);
	colour = other.colour;
	*brush = *(other.brush);

	transparent = other.transparent;
	side_count = other.side_count;
	radius = other.radius;
	center = other.center;

	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = other.points[i];
}
