#pragma once

#include "Matrix.h"
#include "Shape2D.h"

class MatrixShape2D : public Shape2D {
protected:
	const float SCALE_START = 1;
	const float ANGLE_START = 0;
	const float XT_START = 0;
	const float YT_START = 0;

	Matrix* matrix;
	float angle;
	float scale;
	Point center;

	void init_matrix();
	void init_points();
public:
	MatrixShape2D();
	MatrixShape2D(int side_count);
	MatrixShape2D(int side_count, int radius);
	MatrixShape2D(const MatrixShape2D& copy);
	~MatrixShape2D();

	double get_matrix_element(int row, int col);
	void set_matrix(Matrix* matrix);
	void mult_by_matrix(Matrix* m);
	void set_angle(float angle);
	float get_angle();
	void set_scale(float scale);
	float get_scale();
	void set_center(Point center);
	Point get_center();

	void do_move(double dx, double dy);
	void do_rotate(float d_angle);
	void do_scale(float d_scale);

	MatrixShape2D& operator=(const MatrixShape2D& other);
};

