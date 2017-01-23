#include "Shape2D.h"

Shape2D::Shape2D() : Shape2D(DEFAULT_SIDE_COUNT, DEFAULT_RADIUS) {}

Shape2D::Shape2D(int side_count) : Shape2D(side_count, DEFAULT_RADIUS) {}

Shape2D::Shape2D(int side_count, int radius) {
	colour = Colour();
	brush = nullptr;

	this->side_count = side_count;
	this->radius = radius;
	init_points();
}

Shape2D::Shape2D(const Shape2D & copy) {
	colour = copy.colour;
	*brush = *(copy.brush);
	side_count = copy.side_count;
	radius = copy.radius;
	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = copy.points[i];
}

Shape2D::~Shape2D() {
	if (brush != nullptr)
		delete brush;
	delete[] points;
}

void Shape2D::set_colour(Colour colour) {
	this->colour = colour;
}

Colour Shape2D::get_colour() {
	return colour;
}

void Shape2D::set_side_count(int side_count) {
	this->side_count = side_count;
	init_points();
}

int Shape2D::get_side_count() {
	return side_count;
}

void Shape2D::set_radius(int radius) {
	this->radius = radius;
	init_points();
}

int Shape2D::get_radius() {
	return radius;
}

void Shape2D::set_brush(Line * brush) {
	this->brush = brush;
}

void Shape2D::draw(SDL_Surface * s) {
	for (int i = 0; i < side_count; i++) {
		brush->set_colour(colour);
		brush->draw(s, points[i], points[NEXT(i, side_count)]);
	}
}

Point Shape2D::get_point(int index) {
	return points[index];
}

Shape2D & Shape2D::operator=(const Shape2D & other) {
	if (&other == this)
		return (*this);

	colour = other.colour;
	*brush = *(other.brush);
	side_count = other.side_count;
	radius = other.radius;
	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = other.points[i];
}

void Shape2D::init_points() {
	if (points != nullptr)
		delete[] points;
	points = new Point[side_count];
	int start_angle = abs(90 - (360 / side_count) / 2);
	for (int i = 0; i < side_count; i++) {
		points[i] = Point(
			radius * cos(2 * M_PI * i / side_count + RAD(start_angle)),
			radius * sin(2 * M_PI * i / side_count + RAD(start_angle))
		);
	}
}