#include "CyrusBeckLine.h"

CyrusBeckLine::CyrusBeckLine() : CyrusBeckLine(Colour()) {}

CyrusBeckLine::CyrusBeckLine(Colour colour) {
	this->colour = colour;
	basic_brush = nullptr;
}

CyrusBeckLine::CyrusBeckLine(const CyrusBeckLine & copy) {
	colour = copy.colour;
	*basic_brush = *(copy.basic_brush);
	sections = copy.sections;
}

CyrusBeckLine::~CyrusBeckLine() {
	delete basic_brush;
}

void CyrusBeckLine::push_section_window(SectionWindow section) {
	section_count++;
	sections.push_back(section);
}

SectionWindow CyrusBeckLine::pop_section_window() {
	SectionWindow result = sections.at(section_count - 1);
	sections.pop_back();
	return result;
}

int CyrusBeckLine::get_section_window_count() {
	return section_count;
}

void CyrusBeckLine::set_basic_brush(Line * brush) {
	basic_brush = brush;
}

void CyrusBeckLine::draw(SDL_Surface * s, Point start, Point finish) {
	optimize_sections();
	count_drawing_coef();
	basic_brush->set_colour(colour);
	draw_with_section_window(s, start, finish, 0, 0);
}

CyrusBeckLine & CyrusBeckLine::operator=(const CyrusBeckLine & other) {
	if (&other == this)
		return (*this);

	colour = other.colour;
	*basic_brush = *(other.basic_brush);
	sections = other.sections;

	return (*this);
}

float CyrusBeckLine::scalar_mult(Vector p1, Vector p2) {
	return p1.x * p2.x + p1.y * p2.y;
}

void CyrusBeckLine::draw_with_section_window(SDL_Surface * s, Point start, Point finish, int section_number, int visible_count) {
	if (start == finish)
		return;
	if (section_number == section_count) {
		if (visible_count == drawing_coef)
			basic_brush->draw(s, start, finish);
		return;
	}
	bool current_transparency = sections.at(section_number).is_transparent();

	float t_low = 0, t_high = 1, t;
	Point directrix = finish - start;
	int current_side_count = sections.at(section_number).get_side_count();
	for (int i = 0; i < current_side_count; i++) {
		Point sw_side_start = sections.at(section_number).get_point(i);
		Point sw_side_finish = sections.at(section_number).get_point(NEXT(i, current_side_count));
		Vector normal = Vector(sw_side_start.y - sw_side_finish.y,
							   sw_side_finish.x - sw_side_start.x);
		Vector w = start - sw_side_start;
		float d_scalar = scalar_mult(directrix, normal);
		float w_scalar = scalar_mult(w, normal);

		if (abs(d_scalar) < ALMOST_ZERO) {
			if (w_scalar < -ALMOST_ZERO) {
				t_low = 2 * t_high;
				break;
			}
			continue;
		}
		t = -w_scalar / d_scalar;
		if (d_scalar > ALMOST_ZERO) {
			if (t > 1 + ALMOST_ZERO) {
				t_low = 2 * t_high;
				break;
			}
			t_low = max(t, t_low);
			continue;
		}
		if (t < -ALMOST_ZERO) {
			t_low = 2 * t_high;
			break;
		}
		t_high = min(t, t_high);
	}

	int coef = current_transparency ? 1 : -1;
	if (t_low <= t_high) {
		Point lower_point = start + (finish - start) * t_low;
		Point higher_point = start + (finish - start) * t_high;

		draw_with_section_window(s, start, lower_point, section_number + 1, visible_count - coef);
		draw_with_section_window(s, lower_point, higher_point, section_number + 1, visible_count + coef);
		draw_with_section_window(s, higher_point, finish, section_number + 1, visible_count - coef);
	} else {
		draw_with_section_window(s, start, finish, section_number + 1, visible_count - coef);
	}
}

void CyrusBeckLine::count_drawing_coef() {
	drawing_coef = 0;
	if (section_count % 2 == 0) {
		drawing_coef = sections.at(section_count - 1).is_transparent() ? 2 : 0;
	} else {
		drawing_coef = 1;
	}
}

void CyrusBeckLine::optimize_sections() {
	std::vector<int> sections_counts_to_keep;
	std::vector<SectionWindow> optimized_sections;

	bool prev_transp = sections.at(0).is_transparent();
	int dif_transp = 0;
	for (int i = 1; i < section_count; i++) {
		if (sections.at(i).is_transparent() != prev_transp) {
			prev_transp = sections.at(i).is_transparent();
			sections_counts_to_keep.push_back(dif_transp);
		} 
		dif_transp = i;
	}
	sections_counts_to_keep.push_back(dif_transp);

	for (int i = 0; i < sections_counts_to_keep.size(); i++) {
		optimized_sections.push_back(sections.at(sections_counts_to_keep.at(i)));
	}

	section_count = optimized_sections.size();
	sections = optimized_sections;
}
