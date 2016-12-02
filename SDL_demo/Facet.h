#pragma once

#include "Point.h"
#include "Matrix.h"
#include <vector>

class Facet {
protected:
	std::vector<Point> points;
public:
	Facet();
	Facet(const Facet& copy);
	~Facet();

	void add_point(Point point);
	Point get_point(int count);
	int get_side_count();

	void recount(Matrix* matrix);
	Point get_closest_point();
	bool is_visible(Vector observer);

	Facet& operator=(const Facet& other);
};

