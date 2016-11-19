#include "NestedShape2D.h"

NestedShape2D::NestedShape2D() : NestedShape2D(DEFAULT_SIDE_COUNT, DEFAULT_RADIUS) {}

NestedShape2D::NestedShape2D(int side_count) : NestedShape2D(side_count, DEFAULT_RADIUS) {}

NestedShape2D::NestedShape2D(int side_count, int radius) {
	matrix = Matrix(3);
	colour = Colour();
	brush = nullptr;

	nested_count = DEFAULT_NESTED_COUNT;
	this->side_count = side_count;
	this->radius = radius;
	init_points();

	center = Point();
	angle = 0.0f;
	scale = 0.0f;
}

NestedShape2D::NestedShape2D(const NestedShape2D & copy) {
	matrix = copy.matrix;
	colour = copy.colour;
	*brush = *(copy.brush);

	nested_count = copy.nested_count;
	side_count = copy.side_count;
	radius = copy.radius;
	center = copy.center;

	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = copy.points[i];
}

NestedShape2D::~NestedShape2D() {}

void NestedShape2D::set_nested_count(int nested_count) {
	this->nested_count = nested_count;
}

int NestedShape2D::get_nested_count() {
	return nested_count;
}

void NestedShape2D::draw(SDL_Surface * s) {
	brush->set_colour(colour);
	float tmp_tan = tan(M_PI / 4 * MULTIPLICITY / nested_count);
	float side_separator_coef = tmp_tan / (tmp_tan + 1);
	for (int i = 0; i < nested_count; i++) {
		for (int j = 0; j < side_count; j++)
			brush->draw(s, points[j], points[NEXT(j, side_count)]);
		recount_coordinates(side_separator_coef);
	}
}

NestedShape2D & NestedShape2D::operator=(const NestedShape2D & other) {
	if (&other == this)
		return (*this);

	matrix = other.matrix;
	colour = other.colour;
	*brush = *(other.brush);

	nested_count = other.nested_count;
	side_count = other.side_count;
	radius = other.radius;
	center = other.center;

	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = other.points[i];
}


void NestedShape2D::recount_coordinates(float side_separator_coef) {
	Point* new_coordinates = new Point[side_count];

	for (int i = 0; i < side_count; i++) {
		int next = NEXT(i, side_count);
		new_coordinates[next] = Point(
			points[i].x + (points[next].x - points[i].x) * side_separator_coef,
			points[i].y + (points[next].y - points[i].y) * side_separator_coef
		);
	}
	for (int i = 0; i < side_count; i++) 
		points[i] = new_coordinates[i];

	delete[] new_coordinates;
}