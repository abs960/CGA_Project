#include "Shape3D.h"

Shape3D::Shape3D() : Shape3D(Point(), DEFAULT_SIDE_LENGTH) {}

Shape3D::Shape3D(Point pivot, int side_length) {
	this->pivot = pivot;
	this->side_length = side_length;
	init_facets();
}

Shape3D::Shape3D(const Shape3D & copy) {
	side_length = copy.side_length;
	facet_count = copy.facet_count;
	pivot = copy.pivot;

	facets = new Facet[facet_count];
	for (int i = 0; i < facet_count; i++)
		facets[i] = copy.facets[i];
}

Shape3D::~Shape3D() {
	delete[] facets;
}

Facet Shape3D::get_facet(int count) {
	return facets[count];
}

int Shape3D::get_facet_count() {
	return facet_count;
}

void Shape3D::set_side_length(int side_length) {
	this->side_length = side_length;
	init_facets();
}

int Shape3D::get_side_length() {
	return side_length;
}

void Shape3D::recount(Matrix * matrix) {
	for (int i = 0; i < facet_count; i++) {
		facets[i].recount(matrix);
	}
}

std::vector<Facet> Shape3D::get_visible_facets(Vector observer) {
	std::vector<Facet> res;
	for (int i = 0; i < facet_count; i++)
		if (facets[i].is_visible(observer))
			res.push_back(facets[i]);
	return res;
}

Shape3D & Shape3D::operator=(const Shape3D & other) {
	if (&other == this)
		return *this;

	pivot = other.pivot;
	facet_count = other.facet_count;
	side_length = other.side_length;

	for (int i = 0; i < facet_count; i++)
		facets[i] = other.facets[i];

	return *this;
}

void Shape3D::init_facets() {
	Point* points = new Point[8];

	points[0] = pivot + Point(0,			0,				0);
	points[1] = pivot + Point(side_length,	0,				0);
	points[2] = pivot + Point(side_length,	0,				-side_length);
	points[3] = pivot + Point(0,			0,				-side_length);
	points[4] = pivot + Point(0,			side_length,	0);
	points[5] = pivot + Point(side_length,	side_length,	0);
	points[6] = pivot + Point(side_length,	side_length,	-side_length);
	points[7] = pivot + Point(0,			side_length,	-side_length);

	facet_count = 6;
	facets = new Facet[facet_count];

	add_points_to_facet(0, { points[0], points[1], points[2], points[3] });
	add_points_to_facet(1, { points[0], points[4], points[5], points[1] });
	add_points_to_facet(2, { points[0], points[3], points[7], points[4] });
	add_points_to_facet(3, { points[1], points[5], points[6], points[2] });
	add_points_to_facet(4, { points[5], points[4], points[7], points[6] });
	add_points_to_facet(5, { points[3], points[2], points[6], points[7] });

	delete[] points;
}

void Shape3D::add_points_to_facet(int facet_num, std::vector<Point> points) {
	for (int i = 0; i < points.size(); i++)
		facets[facet_num].add_point(points.at(i));
}