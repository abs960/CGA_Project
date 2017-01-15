#pragma once

#include "stdafx.h"

typedef class Point {
private:
	bool equals(float f1, float f2);
protected:
	static const int DEFAULT_COORDINATE = 0;
public:
	float x;
	float y;
	float z;

	Point();
	Point(double x, double y);
	Point(double x, double y, double z);
	Point(std::vector<double> v);
	Point(const Point& copy);
	~Point();

	Point& operator=(const Point& other);

	Point operator+(Point p);
	Point& operator+=(Point p);
	Point operator-(Point p);
	Point& operator-=(Point p);
	Point operator*(double m);
	Point& operator*=(double m);
	double operator*(Point p);
	Point operator/(double d);
	Point& operator/=(double d);
	
	bool operator==(Point p);
	bool operator!=(Point p);

	Point vector_mult(Point p);
	double modulus();
	Point get_normalized();
	
	void from_vector(std::vector<double> v);
	std::vector<double> to_vector();
} Point, Vector;