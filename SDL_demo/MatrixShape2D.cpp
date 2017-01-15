#include "MatrixShape2D.h"

MatrixShape2D::MatrixShape2D() : MatrixShape2D(DEFAULT_SIDE_COUNT, DEFAULT_RADIUS) {}

MatrixShape2D::MatrixShape2D(int side_count) : MatrixShape2D(side_count, DEFAULT_RADIUS) {}

MatrixShape2D::MatrixShape2D(int side_count, int radius) {
	matrix = new Matrix(3);
	matrix->make_identity_matrix();
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
	*matrix = *(copy.matrix);
	colour = copy.colour;
	*brush = *(copy.brush);

	side_count = copy.side_count;
	radius = copy.radius;
	center = copy.center;

	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = copy.points[i];
}

MatrixShape2D::~MatrixShape2D() {
	delete matrix;
}

double MatrixShape2D::get_matrix_element(int row, int col) {
	return matrix->get_element(row, col);
}

void MatrixShape2D::set_matrix(Matrix* matrix) {
	this->matrix = matrix;
	init_points();
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

void MatrixShape2D::do_move(double dx, double dy) {
	center = center + Point(dx, dy);

	Matrix d_matrix = Matrix(3);
	d_matrix.set_element(0, 0, 1);
	d_matrix.set_element(0, 2, dx);
	d_matrix.set_element(1, 1, 1);
	d_matrix.set_element(1, 2, dy);
	d_matrix.set_element(2, 2, 1);

	*matrix *= d_matrix;
	init_points();
}

void MatrixShape2D::do_rotate(float d_angle) {
	angle += d_angle;
	
	Matrix d_matrix = Matrix(3);
	d_matrix.set_element(0, 0, cos(RAD(d_angle)));
	d_matrix.set_element(0, 1, -sin(RAD(d_angle)));
	d_matrix.set_element(1, 0, sin(RAD(d_angle)));
	d_matrix.set_element(1, 1, cos(RAD(d_angle)));
	d_matrix.set_element(2, 2, 1);

	*matrix *= d_matrix;
	init_points();
}

void MatrixShape2D::do_scale(float d_scale) {
	scale += d_scale;

	Matrix d_matrix = Matrix(3);
	d_matrix.set_element(0, 0, 1 + d_scale);
	d_matrix.set_element(1, 1, 1 + d_scale);
	d_matrix.set_element(2, 2, 1);

	*matrix *= d_matrix;
	init_points();
}

void MatrixShape2D::init_points() {
	points = new Point[side_count];

	int start_angle = abs(90 - (360 / side_count) / 2);
	for (int i = 0; i < side_count; i++) {
		std::vector<double> old_coords;
		old_coords.push_back(radius * cos(2 * M_PI * i / side_count + RAD(start_angle)));
		old_coords.push_back(radius * sin(2 * M_PI * i / side_count + RAD(start_angle)));
		old_coords.push_back(1);
		std::vector<double> new_coords = (*matrix) * old_coords;
		points[i].from_vector(new_coords);
	}
}

MatrixShape2D & MatrixShape2D::operator=(const MatrixShape2D & other) {
	if (&other == this)
		return (*this);

	*matrix = *(other.matrix);
	colour = other.colour;
	*brush = *(other.brush);
	side_count = other.side_count;
	radius = other.radius;
	center = other.center;

	points = new Point[side_count];
	for (int i = 0; i < side_count; i++)
		points[i] = other.points[i];
}