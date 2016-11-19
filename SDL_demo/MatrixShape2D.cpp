#include "MatrixShape2D.h"

MatrixShape2D::MatrixShape2D() : MatrixShape2D(DEFAULT_SIDE_COUNT, DEFAULT_RADIUS) {}

MatrixShape2D::MatrixShape2D(int side_count) : MatrixShape2D(side_count, DEFAULT_RADIUS) {}

MatrixShape2D::MatrixShape2D(int side_count, int radius) {
	matrix = Matrix(3);
	colour = Colour();
	brush = nullptr;

	this->side_count = side_count;
	this->radius = radius;
	init_points();

	center = Point();
	angle = 0.0f;
	scale = 0.0f;
}

MatrixShape2D::MatrixShape2D(const MatrixShape2D & copy) {
	matrix = copy.matrix;
	colour = copy.colour;
	*brush = *(copy.brush);

	side_count = copy.side_count;
	radius = copy.radius;
	center = copy.center;

	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = copy.points[i];
}

MatrixShape2D::~MatrixShape2D() {}

double MatrixShape2D::get_element(int row, int col) {
	return matrix.get_element(row, col);
}

void MatrixShape2D::set_matrix(Matrix matrix) {
	this->matrix = matrix;
	apply_matrix();
}

void MatrixShape2D::set_angle(float angle) {
	this->angle = angle;
}

float MatrixShape2D::get_angle() {
	return angle;
}

void MatrixShape2D::set_scale(float scale) {
	this->scale = scale;
}

float MatrixShape2D::get_scale() {
	return scale;
}

void MatrixShape2D::set_center(Point center) {
	this->center = center;
}

Point MatrixShape2D::get_center() {
	return center;
}

void MatrixShape2D::apply_matrix() {
	for (int i = 0; i < side_count; i++) {
		std::vector<double> counted_coordinates;
		counted_coordinates.push_back(points[i].x);
		counted_coordinates.push_back(points[i].y);
		counted_coordinates.push_back(1);

		std::vector<double> result = matrix * counted_coordinates;

		points[i].x = result[0];
		points[i].y = result[1];
	}
}

MatrixShape2D & MatrixShape2D::operator=(const MatrixShape2D & other) {
	if (&other == this)
		return (*this);

	matrix = other.matrix;
	colour = other.colour;
	*brush = *(other.brush);

	side_count = other.side_count;
	radius = other.radius;
	center = other.center;

	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = other.points[i];
}