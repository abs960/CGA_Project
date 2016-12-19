#include "Quaternion.h"

Quaternion::Quaternion() : Quaternion(0, Vector(1, 0, 0)) {}

Quaternion::Quaternion(double angle, Vector rotation_axis) {
	set_rotation_axis(angle, rotation_axis);
}

Quaternion::Quaternion(const Quaternion & copy) {
	rotation = copy.rotation;
	q_vector = copy.q_vector;
}

Quaternion::~Quaternion() {}

void Quaternion::set_rotation_axis(double angle, Vector axis) {
	rotation = cos(RAD(angle / 2));
	Vector normalized = axis.get_normalized();
	q_vector.x = normalized.x * sin(RAD(angle / 2));
	q_vector.y = normalized.y * sin(RAD(angle / 2));
	q_vector.z = normalized.z * sin(RAD(angle / 2));
}

double Quaternion::get_w() {
	return rotation;
}

double Quaternion::get_x() {
	return q_vector.x;
}

double Quaternion::get_y() {
	return q_vector.y;
}

double Quaternion::get_z() {
	return q_vector.z;
}

void Quaternion::invert() {
	q_vector.x = -q_vector.x;
	q_vector.y = -q_vector.y;
	q_vector.z = -q_vector.z;

	normalize();
}

void Quaternion::normalize() {
	double len = get_length();
	Quaternion q = *this * (1 / len);
	*this = q;
}

double Quaternion::get_length() {
	return sqrt(rotation * rotation + q_vector.x * q_vector.x +
				q_vector.y * q_vector.y + q_vector.z * q_vector.z);
}

Vector Quaternion::transform(Vector v) {
	Quaternion inverted = *this;
	inverted.invert();
	Quaternion tmp = *this * v;
	Quaternion res = tmp * inverted;

	Vector result = Vector();
	result.x = res.q_vector.x;
	result.y = res.q_vector.y;
	result.z = res.q_vector.z;

	return result;
}

Matrix Quaternion::to_matrix() {
	double w = rotation;
	double x = q_vector.x;
	double y = q_vector.y;
	double z = q_vector.z;

	Matrix tmp = Matrix(4);
	tmp.set_element(0, 0, 1 - 2 * (y * y + z * z));
	tmp.set_element(1, 0, 2 * (x * y - z * w));
	tmp.set_element(2, 0, 2 * (x * z + y * w));
	tmp.set_element(3, 0, 0);

	tmp.set_element(0, 1, 2 * (x * y + z * w));
	tmp.set_element(1, 1, 1 - 2 * (x * x + z * z));
	tmp.set_element(2, 1, 2 * (y * z - x * w));
	tmp.set_element(3, 1, 0);

	tmp.set_element(0, 2, 2 * (x * z - y * w));
	tmp.set_element(1, 2, 2 * (y * z + x * w));
	tmp.set_element(2, 2, 1 - 2 * (x * x + y * y));
	tmp.set_element(3, 2, 0);

	tmp.set_element(0, 3, 0);
	tmp.set_element(1, 3, 0);
	tmp.set_element(2, 3, 0);
	tmp.set_element(3, 3, 1);

	return tmp;
}

Quaternion & Quaternion::operator=(const Quaternion & other) {
	if (&other == this)
		return *this;

	rotation = other.rotation;
	q_vector = other.q_vector;

	return *this;
}

Quaternion Quaternion::operator*(Quaternion q) {
	Quaternion res = Quaternion();

	res.rotation =		rotation * q.rotation	- q_vector.x * q.q_vector.x		- q_vector.y * q.q_vector.y		- q_vector.z * q.q_vector.z;
	res.q_vector.x =	rotation * q.q_vector.x	+ q_vector.x * q.rotation		+ q_vector.y * q.q_vector.z		- q_vector.z * q.q_vector.y;
	res.q_vector.y =	rotation * q.q_vector.y	- q_vector.x * q.q_vector.z		+ q_vector.y * q.rotation		+ q_vector.z * q.q_vector.x;
	res.q_vector.z =	rotation * q.q_vector.z	+ q_vector.x * q.q_vector.y		- q_vector.y * q.q_vector.x		+ q_vector.z * q.rotation;

	return res;
}

Quaternion Quaternion::operator*(Vector v) {
	Quaternion res;

	res.rotation = -q_vector.x * v.x - q_vector.y * v.y - q_vector.z * v.z;
	res.q_vector.x = rotation * v.x + q_vector.y * v.z - q_vector.z * v.y;
	res.q_vector.y = rotation * v.y - q_vector.x * v.z + q_vector.z * v.x;
	res.q_vector.z = rotation * v.z + q_vector.x * v.y - q_vector.y * v.x;

	return res;
}

Quaternion Quaternion::operator*(double d) {
	Quaternion q = Quaternion();

	q.rotation = rotation * d;
	q.q_vector.x = q_vector.x * d;
	q.q_vector.y = q_vector.y * d;
	q.q_vector.z = q_vector.z * d;

	return q;
}