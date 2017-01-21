#pragma once

#include "vector"

class Matrix {
private:
	static const int DEFAULT_WIDTH = 3;
	static const int DEFAULT_HEIGHT = 3;
	int width;
	int height;
	double **elements;

	void copy_elements(double **new_elements);
	void free_elements();
public:
	Matrix();
	Matrix(int size);
	Matrix(int height, int width);
	Matrix(const Matrix &copy);
	~Matrix();

	int get_width();
	int get_height();
	double get_element(int col, int row);
	void set_element(int col, int row, double value);
	void make_identity_matrix();

	Matrix& operator=(const Matrix &other);

	Matrix operator*(const Matrix &other);
	Matrix& operator*=(const Matrix &other);

	std::vector<double> operator*(const std::vector<double> &v);
	std::vector<double>& operator*=(const std::vector<double> &v);
};

