#include "Facet.h"

Facet::Facet() {}

Facet::Facet(const Facet & copy) {
	points = copy.points;
}

Facet::~Facet() {}

void Facet::add_point(Point point) {
	points.push_back(point);
}

Point Facet::get_point(int count) {
	return points.at(count);
}

int Facet::get_side_count() {
	return points.size();
}

void Facet::recount(Matrix * matrix) {
	for (int i = 0; i < points.size(); i++) {
		std::vector<double> v;
		v.push_back(points[i].x);
		v.push_back(points[i].y);
		v.push_back(points[i].z);
		v.push_back(1);
		std::vector<double> res = (*matrix) * v;
		points[i].x = res.at(0);
		points[i].y = res.at(1);
		points[i].z = res.at(2);
	}
}

Point Facet::get_closest_point() {
	float min_z = INFINITY;
	int min_z_count = -1;
	for (int i = 0; i < points.size(); i++) {
		if (points.at(i).z < min_z) {
			min_z = points.at(i).z;
			min_z_count = i;
		}
	}
	return points.at(min_z_count);
}

bool Facet::is_visible(Vector observer) {
	Vector edge0 = points.at(0) - points.at(1);
	Vector edge1 = points.at(1) - points.at(2);
	Vector normal = edge0.vector_mult(edge1);
	double scalar_mult_res = normal * observer;
	return scalar_mult_res > 0;
}

Facet & Facet::operator=(const Facet & other) {
	if (&other == this)
		return *this;

	points = other.points;

	return *this;
}