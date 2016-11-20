#pragma once

#include "Line.h"
#include "SectionWindow.h"
#include "vector"

class CyrusBeckLine : public Line {
protected:
	int drawing_coef;

	int section_count;
	std::vector<SectionWindow> sections;
	Line* basic_brush; 

	float scalar_mult(Vector p1, Vector p2);
	void draw_with_section_window(SDL_Surface* s, Point start, Point finish, int section_number, int visible_count);
	void count_drawing_coef();
	void optimize_sections();
public:
	CyrusBeckLine();
	CyrusBeckLine(Colour colour);
	CyrusBeckLine(const CyrusBeckLine& copy);
	~CyrusBeckLine();

	void push_section_window(SectionWindow section);
	SectionWindow pop_section_window();
	int get_section_window_count();
	void set_basic_brush(Line* brush);

	void draw(SDL_Surface* s, Point start, Point finish);

	CyrusBeckLine& operator=(const CyrusBeckLine& other);
};

