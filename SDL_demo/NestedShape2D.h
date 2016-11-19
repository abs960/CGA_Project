#pragma once

#include "MatrixShape2D.h"

#define DEFAULT_NESTED_COUNT 5
#define MULTIPLICITY 3

class NestedShape2D : public MatrixShape2D {
protected:
	int nested_count;

	void recount_coordinates(Point* work_points, float side_separator_coef);
public:
	NestedShape2D();
	NestedShape2D(int side_count);
	NestedShape2D(int side_count, int radius);
	NestedShape2D(const NestedShape2D& copy);
	~NestedShape2D();

	void set_nested_count(int nested_count);
	int get_nested_count();

	void draw(SDL_Surface* s);

	NestedShape2D& operator=(const NestedShape2D& other);
};

