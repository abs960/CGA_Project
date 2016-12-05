#include "Point.h"

Point::Point() : Point(DEFAULT_COORDINATE, DEFAULT_COORDINATE, DEFAULT_COORDINATE) {}

Point::Point(double x, double y) : Point(x, y, DEFAULT_COORDINATE) {}

Point::Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point(const Point & copy) {
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

Point::~Point() {}

Point & Point::operator=(const Point& other) {
	if (this == &other)
		return (*this);

	x = other.x;
	y = other.y;
	z = other.z;

	return (*this);
}

Point Point::operator+(Point p) {
	return Point(x + p.x, y + p.y, z + p.z);
}

Point & Point::operator+=(Point p) {
	return (*this) + p;
}

Point Point::operator-(Point p) {
	return Point(x - p.x, y - p.y, z - p.z);
}

Point & Point::operator-=(Point p) {
	return (*this) - p;
}

Point Point::operator*(double m) {
	return Point(x * m, y * m, z * m);
}

Point & Point::operator*=(double m) {
	return (*this) * m;
}

double Point::operator*(Point p) {
	return x * p.x + y * p.y + z * p.z;
}

Point Point::operator/(double d) {
	return Point(x / d, y / d, z / d);
}

Point & Point::operator/=(double d) {
	return (*this) / d;
}

bool Point::operator==(Point p) {
	return equals(x, p.x) && equals(y, p.y) && equals(z, p.z);
}

bool Point::operator!=(Point p) {
	return !((*this) == p);
}

Point Point::vector_mult(Point p) {
	return Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
}

double Point::modulus() {
	return sqrt(x * x + y * y + z * z);
}

bool Point::equals(float f1, float f2) {
	return abs(f1 - f2) < ALMOST_ZERO;
}
