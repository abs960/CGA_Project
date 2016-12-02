#pragma once

#include "Point.h"
#include "Facet.h"
#include "Matrix.h"

class Shape3D {
protected:
	const static int DEFAULT_SIDE_LENGTH = 100;

	Point pivot;
	Facet* facets;
	int facet_count;
	int side_length;

	void init_facets();
	void add_points_to_facet(int facet_num, std::vector<Point> points);
public:
	Shape3D();
	Shape3D(Point pivot, int side_length);
	Shape3D(const Shape3D& copy);
	~Shape3D();

	Facet get_facet(int count);
	int get_facet_count();
	void set_side_length(int side_length);
	int get_side_length();

	void recount(Matrix* matrix);
	std::vector<Facet> get_visible_facets(Vector observer);

	Shape3D& operator=(const Shape3D& other);
};

