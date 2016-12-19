#include "QuaternionScene3D.h"

QuaternionScene3D::QuaternionScene3D() {
	colour = Colour();
	lines_visible = true;
	matrix = new Matrix(4);
	quaternion = Quaternion();
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			double element = i == j ? 1 : 0;
			matrix->set_element(i, j, element);
		}

	store_shift = Point(SCREEN_WIDTH / 2 - SIDE_LENGTH, SCREEN_HEIGHT / 2 + SIDE_LENGTH, 0);
	matrix->set_element(0, 3, store_shift.x);
	matrix->set_element(1, 3, store_shift.y);
	matrix->set_element(2, 3, store_shift.z);
}

QuaternionScene3D::QuaternionScene3D(const QuaternionScene3D & copy) {
	colour = copy.colour;
	*base_line = *(copy.base_line);
	objects = copy.objects;
	*matrix = *(copy.matrix);
	quaternion = copy.quaternion;
	lines_visible = copy.lines_visible;
}

QuaternionScene3D::~QuaternionScene3D() {}

void QuaternionScene3D::rotate_x(float d_angle) {
	rotate_vector(Vector(1, 0, 0), d_angle);
}

void QuaternionScene3D::rotate_y(float d_angle) {
	rotate_vector(Vector(0, 1, 0), d_angle);
}

void QuaternionScene3D::rotate_z(float d_angle) {
	rotate_vector(Vector(0, 0, 1), d_angle);
}

void QuaternionScene3D::rotate_vector(Vector vector, double d_angle) {
	save_shift();

	Vector delta_angle = vector.get_normalized() * d_angle;
	angles = angles + delta_angle;

	Quaternion q = Quaternion(d_angle, vector);
	quaternion = quaternion * q;

	load_shift();
}

void QuaternionScene3D::move(float dx, float dy, float dz) {
	Matrix tmp = Matrix(4);
	tmp.set_element(0, 0, 1);
	tmp.set_element(1, 1, 1);
	tmp.set_element(2, 2, 1);
	tmp.set_element(3, 3, 1);
	tmp.set_element(0, 3, dx);
	tmp.set_element(1, 3, dy);
	tmp.set_element(2, 3, dz);
	*matrix *= tmp;
}

QuaternionScene3D & QuaternionScene3D::operator=(const QuaternionScene3D & other) {
	if (&other == this)
		return *this;

	colour = other.colour;
	*base_line = *(other.base_line);
	objects = other.objects;
	*matrix = *(other.matrix);
	quaternion = other.quaternion;
	lines_visible = other.lines_visible;

	return *this;
}

void QuaternionScene3D::apply_transformation() {
	for (int i = 0; i < objects.size(); i++) {
		objects.at(i)->set_angle(angles);
		objects.at(i)->recount(matrix, quaternion);
	}
}