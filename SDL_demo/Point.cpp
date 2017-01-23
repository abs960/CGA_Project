#include "Point.h"

Point::Point() : Point(DEFAULT_COORDINATE, DEFAULT_COORDINATE, DEFAULT_COORDINATE) {}

Point::Point(double x, double y) : Point(x, y, DEFAULT_COORDINATE) {}

Point::Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point(std::vector<double> v) {
	int size = v.size();
	if (size >= 3) {
		x = v.at(0);
		y = v.at(1);
		z = v.at(2);
	} else if (size == 2) {
		x = v.at(0);
		y = v.at(1);
		z = DEFAULT_COORDINATE;
	} else if (size == 1) {
		x = v.at(0);
		y = DEFAULT_COORDINATE;
		z = DEFAULT_COORDINATE;
	}
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

Point Point::get_normalized() {
	double length = modulus();

	if (abs(length) < ALMOST_ZERO)
		return *this;

	Vector normalized;
	normalized.x = x / length;
	normalized.y = y / length;
	normalized.z = z / length;
	return normalized;
}

void Point::from_vector(std::vector<double> v) {
	int size = v.size();
	if (size >= 3) {
		this->x = v.at(0);
		this->y = v.at(1);
		this->z = v.at(2);
	} else if (size == 2) {
		this->x = v.at(0);
		this->y = v.at(1);
	} else if (size == 1) {
		this->x = v.at(0);
	}
}

std::vector<double> Point::to_vector() {
	std::vector<double> res;
	res.push_back(x);
	res.push_back(y);
	res.push_back(z);
	return res;
}

bool Point::equals(float f1, float f2) {
	return abs(f1 - f2) < ALMOST_ZERO;
}
