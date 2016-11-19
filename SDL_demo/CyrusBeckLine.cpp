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

void cyrus_beck_line(SDL_Surface *s, Point start, Point finish, Point *section_window, bool is_drawing_inside, Uint32 colour);
void CyrusBeckLine::draw(SDL_Surface * s, Point start, Point finish) {
	float t_low = 0, t_high = 1, t;
	Point directrix = finish - start;
	for (int i = 0; i < sections.at(0).get_side_count(); i++) {
		Point sect_wnd_side_start = sections.at(0).get_point(i);
		Point sect_wnd_side_finish = sections.at(0).get_point(NEXT(i, sections.at(0).get_side_count()));
		Vector normal = Vector(sect_wnd_side_start.y - sect_wnd_side_finish.y,
							   sect_wnd_side_finish.x - sect_wnd_side_start.x);
		Point sect_wnd_side_point = sect_wnd_side_start;
		Vector w = start - sect_wnd_side_point;
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
	if (t_low <= t_high) {
		Point lower_point = start + (finish - start) * t_low;
		Point higher_point = start + (finish - start) * t_high;
		if (sections.at(0).is_transparent()) {
			basic_brush->draw(s, lower_point, higher_point);
		} else {
			if (start != lower_point) {
				basic_brush->draw(s, start, lower_point);
			}
			if (finish != higher_point) {
				basic_brush->draw(s, higher_point, finish);
			}
		}
	} else if (!sections.at(0).is_transparent())
		basic_brush->draw(s, start, finish);
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