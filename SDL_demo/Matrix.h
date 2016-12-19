#pragma once

#include "vector"

#define DEFAULT_WIDTH 3
#define DEFAULT_HEIGHT 3

class Matrix {
private:
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

	Matrix& operator=(const Matrix &other);

	Matrix operator*(const Matrix &other);
	Matrix& operator*=(const Matrix &other);

	std::vector<double> operator*(const std::vector<double> &v);
	std::vector<double>& operator*=(const std::vector<double> &v);
};

