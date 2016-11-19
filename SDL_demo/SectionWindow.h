#pragma once
#include "MatrixShape2D.h"
class SectionWindow : public MatrixShape2D {
protected:
	bool transparent;
public:
	SectionWindow();
	SectionWindow(int side_count);
	SectionWindow(int side_count, int radius);
	SectionWindow(const SectionWindow& copy);
	~SectionWindow();

	void set_transparent(bool transparent);
	bool is_transparent();

	SectionWindow& operator=(const SectionWindow& other);
};

