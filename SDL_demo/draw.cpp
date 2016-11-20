#include "draw.h"
#include "math.h"
#include "Point.h"
#include "Line.h"
#include "BrezenheimLine.h"
#include "WuLine.h"
#include "Shape2D.h"
#include "MatrixShape2D.h"
#include "NestedShape2D.h"
#include "SectionWindow.h"
#include "CyrusBeckLine.h"

// Extern global variables
int			figures_side_count[DRAWN_FIGURES_COUNT] = { 4 };
int			nested_figure_count[DRAWN_FIGURES_COUNT] = { 9 };
Uint32		colours[DRAWN_FIGURES_COUNT] = { 0x0000FF00 };

int			sect_wnd_side_count;

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 colour) {
	assert(NULL != surface);
	if (x < 0 || x > SCREEN_WIDTH ||
		y < 0 || y > SCREEN_HEIGHT)
		return;

	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = colour;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y) {
	assert(NULL != surface);
	if (x < 0 || x > SCREEN_WIDTH ||
		y < 0 || y > SCREEN_HEIGHT)
		return -1;

	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}

void draw(SDL_Surface *s, Matrix mtrx_finals[DRAWN_FIGURES_COUNT], bool draw_inside)
{
	sect_wnd_side_count = figures_side_count[0];

	int sect_wnd_radius = RADIUS / 2;
	int sect_wnd_used = USE_SECTION_WINDOW ? 1 : 0;
	SectionWindow sw = SectionWindow(sect_wnd_side_count, sect_wnd_radius);
	SectionWindow sw1 = SectionWindow(sect_wnd_side_count, sect_wnd_radius * 2);
	SectionWindow sw2 = SectionWindow(sect_wnd_side_count, sect_wnd_radius * 3);
	SectionWindow sw3 = SectionWindow(sect_wnd_side_count, sect_wnd_radius * 4);
	SectionWindow sw4 = SectionWindow(sect_wnd_side_count, sect_wnd_radius * 5);
	SectionWindow sw5 = SectionWindow(sect_wnd_side_count, sect_wnd_radius * 6);
	if (USE_SECTION_WINDOW) {
		sw.set_transparent(draw_inside);
		sw.set_colour(Colour(Colour::COLOUR_BLUE));
		sw.set_matrix(mtrx_finals[0]);
		sw.set_brush(new WuLine());
		sw.draw(s);

		sw1.set_transparent(!draw_inside);
		sw1.set_colour(Colour(Colour::COLOUR_BLUE));
		sw1.set_matrix(mtrx_finals[0]);
		sw1.set_brush(new WuLine());
		sw1.draw(s);

		sw2.set_transparent(draw_inside);
		sw2.set_colour(Colour(Colour::COLOUR_BLUE));
		sw2.set_matrix(mtrx_finals[0]);
		sw2.set_brush(new WuLine());
		sw2.draw(s);

		sw3.set_transparent(!draw_inside);
		sw3.set_colour(Colour(Colour::COLOUR_BLUE));
		sw3.set_matrix(mtrx_finals[0]);
		sw3.set_brush(new WuLine());
		sw3.draw(s);

		sw4.set_transparent(draw_inside);
		sw4.set_colour(Colour(Colour::COLOUR_BLUE));
		sw4.set_matrix(mtrx_finals[0]);
		sw4.set_brush(new WuLine());
		sw4.draw(s);

		sw5.set_transparent(!draw_inside);
		sw5.set_colour(Colour(Colour::COLOUR_BLUE));
		sw5.set_matrix(mtrx_finals[0]);
		sw5.set_brush(new WuLine());
		sw5.draw(s);
	}

	for (int i = 0; i < DRAWN_FIGURES_COUNT - sect_wnd_used; i++) {
		NestedShape2D sh = NestedShape2D(figures_side_count[i + sect_wnd_used], RADIUS);
		sh.set_nested_count(nested_figure_count[i + sect_wnd_used]);
		sh.set_matrix(mtrx_finals[i + sect_wnd_used]);
		sh.set_colour(colours[i + sect_wnd_used]);

		if (USE_SECTION_WINDOW) {
			CyrusBeckLine* l = new CyrusBeckLine();
			l->set_basic_brush(new WuLine());

			l->push_section_window(sw);
			l->push_section_window(sw1);
			l->push_section_window(sw2);
			l->push_section_window(sw3);
			l->push_section_window(sw4);
			l->push_section_window(sw5);

			sh.set_brush(l);
		} else
			sh.set_brush(new WuLine());
		sh.draw(s);
	}
}