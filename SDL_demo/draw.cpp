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

	int sect_wnd_radius = RADIUS * 2;
	int sect_wnd_used = USE_SECTION_WINDOW ? 1 : 0;
	SectionWindow sw = SectionWindow(sect_wnd_side_count, sect_wnd_radius);
	if (USE_SECTION_WINDOW) {
		sw.set_transparent(draw_inside);
		sw.set_colour(Colour(Colour::COLOUR_BLUE));
		sw.set_matrix(mtrx_finals[0]);
		sw.set_brush(new WuLine());
		sw.draw(s);
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
			sh.set_brush(l);
		} else
			sh.set_brush(new WuLine());
		sh.draw(s);
	}
}