#pragma once

#include "Matrix.h"
#include "Shape2D.h"

class MatrixShape2D : public Shape2D {
protected:
	Matrix matrix;

	float angle;
	float scale;
	Point center;
public:
	MatrixShape2D();
	MatrixShape2D(int side_count);
	MatrixShape2D(int side_count, int radius);
	MatrixShape2D(const MatrixShape2D& copy);
	~MatrixShape2D();

	double get_element(int row, int col);
	void set_matrix(Matrix matrix);
	void set_angle(float angle);
	float get_angle();
	void set_scale(float scale);
	float get_scale();
	void set_center(Point center);
	Point get_center();
	void apply_matrix();

	MatrixShape2D& operator=(const MatrixShape2D& other);
};

