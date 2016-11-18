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

Point Point::operator/(double d) {
	return Point(x / d, y / d, z / d);
}

Point & Point::operator/=(double d) {
	return (*this) / d;
}

bool Point::operator==(Point p) {
	return x == p.x && y == p.y && z == p.z;
}

bool Point::operator!=(Point p) {
	return !((*this) == p);
}
