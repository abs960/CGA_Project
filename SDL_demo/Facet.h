#pragma once

#include "Point.h"
#include "Line.h"
#include "Matrix.h"
#include <vector>
#include "Quaternion.h"

class Facet {
protected:
	std::vector<Point> points;
public:
	Facet();
	Facet(const Facet& copy);
	~Facet();

	void add_point(Point point);
	Point get_point(int count);
	int get_point_count();

	void recount(Matrix* matrix);
	void recount(Quaternion q);
	void recount(Matrix* matrix, Quaternion q);
	void apply_perspective(Point projection_center, Vector angles);
	Point get_closest_point();
	bool is_visible(Vector observer);
	Vector get_normal();
	Point left();
	Point right();
	Point top();
	Point bottom();
	bool contains(Point p);
	std::vector<int> get_x_borders(int y);

	void fill(SDL_Surface* s, Colour colour, Vector observer);
	void draw(SDL_Surface* s, Line* brush);

	Facet& operator=(const Facet& other);
};

