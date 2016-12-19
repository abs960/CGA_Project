#include "PerspectiveShape3D.h"

PerspectiveShape3D::PerspectiveShape3D() : PerspectiveShape3D(Point(), DEFAULT_SIDE_LENGTH) {}

PerspectiveShape3D::PerspectiveShape3D(Point pivot, int side_length) {
	this->pivot = pivot;
	this->side_length = side_length;
	projection_center = Point(/*-SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2*/0, 0, -500);
	init_facets();
}

PerspectiveShape3D::PerspectiveShape3D(const PerspectiveShape3D & copy) {
	side_length = copy.side_length;
	facet_count = copy.facet_count;
	pivot = copy.pivot;
	projection_center = copy.projection_center;
	angles = copy.angles;

	facets = new Facet[facet_count];
	for (int i = 0; i < facet_count; i++)
		facets[i] = copy.facets[i];
}

PerspectiveShape3D::~PerspectiveShape3D() {}

void PerspectiveShape3D::recount(Matrix * matrix) {
	for (int i = 0; i < facet_count; i++) {
		facets[i].apply_perspective(projection_center, angles);
		facets[i].recount(matrix);
	}
}

void PerspectiveShape3D::recount(Quaternion q) {
	for (int i = 0; i < facet_count; i++) {
		facets[i].apply_perspective(projection_center, angles);
		facets[i].recount(q);
	}
}

void PerspectiveShape3D::recount(Matrix * matrix, Quaternion q) {
	for (int i = 0; i < facet_count; i++) {
		facets[i].apply_perspective(projection_center, angles);
		facets[i].recount(matrix, q);
	}
}

PerspectiveShape3D & PerspectiveShape3D::operator=(const PerspectiveShape3D & other) {
	if (&other == this)
		return *this;

	pivot = other.pivot;
	facet_count = other.facet_count;
	side_length = other.side_length;
	projection_center = other.projection_center;
	angles = other.angles;

	for (int i = 0; i < facet_count; i++)
		facets[i] = other.facets[i];

	return *this;
}