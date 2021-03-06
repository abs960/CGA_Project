#include "Matrix.h"

Matrix::Matrix() : Matrix(DEFAULT_HEIGHT, DEFAULT_WIDTH) {}

Matrix::Matrix(int size) : Matrix(size, size) {}

Matrix::Matrix(int height, int width) {
	this->height = height;
	this->width = width;
	elements = new double*[height];
	for (int i = 0; i < height; i++) {
		elements[i] = new double[width];
		for (int j = 0; j < width; j++)
			elements[i][j] = 0;
	}
}

Matrix::Matrix(const Matrix& copy) {
	width = copy.width;
	height = copy.height;
	copy_elements(copy.elements);
}

Matrix::~Matrix() {
	free_elements();
}

int Matrix::get_width() {
	return width;
}

int Matrix::get_height() {
	return height;
}

double Matrix::get_element(int col, int row) {
	return elements[col][row];
}

void Matrix::set_element(int col, int row, double value) {
	elements[col][row] = value;
}

void Matrix::make_identity_matrix() {
	int count = height < width ? height : width;
	for (int i = 0; i < count; i++)
		for (int j = 0; j < count; j++)
			elements[i][j] = (i == j) ? 1 : 0;
}

Matrix& Matrix::operator=(const Matrix& other) {
	if (&other == this)
		return *this;

	height = other.height;
	width = other.width;

	copy_elements(other.elements);

	return *this;
}

Matrix Matrix::operator*(const Matrix& other) {
	unsigned rows = this->height;
	unsigned cols = other.width;
	Matrix result(rows, cols);

	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
			double sum = 0;
			for (unsigned k = 0; k < other.height; k++) 
				sum += this->elements[i][k] * other.elements[k][j];
			result.set_element(i, j, sum);
		}
	}

	return result;
}

Matrix& Matrix::operator*=(const Matrix& other) {
	Matrix result = (*this) * other;
	(*this) = result;
	return *this;
}

std::vector<double> Matrix::operator*(const std::vector<double>& v) {
	std::vector<double> result(height, 0);

	for (unsigned i = 0; i < height; i++) {
		double sum = 0;
		for (unsigned j = 0; j < v.size(); j++)
			sum += this->elements[i][j] * v.at(j);
		result[i] = sum;
	}

	return result;
}

std::vector<double>& Matrix::operator*=(const std::vector<double>& v) {
	return (*this) * v;
}

void Matrix::copy_elements(double **new_elements) {
	elements = new double*[height];
	for (int i = 0; i < height; i++) {
		elements[i] = new double[width];
		for (int j = 0; j < width; j++)
			elements[i][j] = new_elements[i][j];
	}
}

void Matrix::free_elements() {
	delete[] elements;
}
