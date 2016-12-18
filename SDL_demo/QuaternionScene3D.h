#pragma once

#include "Scene3D.h"
#include "Quaternion.h"

class QuaternionScene3D : public Scene3D {
public:
	QuaternionScene3D();
	QuaternionScene3D(const QuaternionScene3D& copy);
	~QuaternionScene3D();

	virtual void rotate_x(float d_angle);
	virtual void rotate_y(float d_angle);
	virtual void rotate_z(float d_angle);
	virtual void rotate_vector(Vector vector, double d_angle);

	QuaternionScene3D& operator=(const QuaternionScene3D& other);
};

