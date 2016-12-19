#pragma once

#include "Shape3D.h"

class PerspectiveShape3D : public Shape3D {
protected:
	Point projection_center;
public:
	PerspectiveShape3D();
	PerspectiveShape3D(Point pivot, int side_length);
	PerspectiveShape3D(const PerspectiveShape3D& copy);
	~PerspectiveShape3D();

	void recount(Matrix* matrix);
	void recount(Quaternion q);
	void recount(Matrix* matrix, Quaternion q);

	PerspectiveShape3D& operator=(const PerspectiveShape3D& other);
};

