#pragma once

#include "Point.h"
#include "Matrix.h"

class Quaternion {
protected:
	double rotation;
	Vector q_vector;
public:

	Quaternion();
	Quaternion(double angle, Vector rotation_axis);
	Quaternion(const Quaternion& copy);
	~Quaternion();

	void set_rotation_axis(double angle, Vector axis);
	double get_w();
	double get_x();
	double get_y();
	double get_z();

	void invert();
	void normalize();
	double get_length();
	Vector transform(Vector v);
	Matrix to_matrix();

	Quaternion& operator=(const Quaternion& other);
	Quaternion operator*(Quaternion q);
	Quaternion operator*(Vector v);
	Quaternion operator*(double d);
};

