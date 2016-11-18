#pragma once

#define DEFAULT_COORDINATE 0

typedef class Point {
public:
	double x;
	double y;
	double z;

	Point();
	Point(double x, double y);
	Point(double x, double y, double z);
	Point(const Point& copy);
	~Point();

	Point operator+(Point p);
	Point& operator+=(Point p);
	Point operator-(Point p);
	Point& operator-=(Point p);
	Point operator*(double m);
	Point& operator*=(double m);
	Point operator/(double d);
	Point& operator/=(double d);
	
	bool operator==(Point p);
	bool operator!=(Point p);
} Point, Vector;