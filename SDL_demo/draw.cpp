#include "draw.h"
#include "math.h"
#include "Point.h"
#include "Line.h"
#include "BrezenheimLine.h"
#include "WuLine.h"

Point recount_point(Point point, Matrix mtrxF);

const int	MULTIPLICITY = 3;

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

void init_start_coordinates(Point* start, int side_count, int radius, Matrix mtrx_final) {
	int start_angle = abs(90 - (360 / side_count) / 2);
	for (int i = 0; i < side_count; i++) {
		Point tmp = Point(
			radius * cos(2 * M_PI * i / side_count + RAD(start_angle)),
			radius * sin(2 * M_PI * i / side_count + RAD(start_angle))
		);
		start[i] = recount_point(tmp, mtrx_final);
	}
}

float scalar_mult(Point p1, Point p2) {
	return p1.x * p2.x + p1.y * p2.y;
}

void cyrus_beck_line(SDL_Surface *s, Point start, Point finish, Point *section_window, bool is_drawing_inside, Uint32 colour) {
	float t_low = 0, t_high = 1, t;
	Point directrix = finish - start;
	for (int i = 0; i < sect_wnd_side_count; i++) {
		Point sect_wnd_side_start = section_window[i];
		Point sect_wnd_side_finish = section_window[NEXT(i, sect_wnd_side_count)];
		Point normal = Point(sect_wnd_side_start.y - sect_wnd_side_finish.y, 
							 sect_wnd_side_finish.x - sect_wnd_side_start.x);
		Point sect_wnd_side_point = sect_wnd_side_start;
		Point w = start - sect_wnd_side_point;
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
	WuLine result = WuLine(Colour(colour));
	if (t_low <= t_high) {
		Point lower_point = start + (finish - start) * t_low;
		Point higher_point = start + (finish - start) * t_high;
		if (is_drawing_inside) {
			result.draw(s, lower_point, higher_point);
		} else {
			if (start != lower_point) {
				result.draw(s, start, lower_point);
			}
			if (finish != higher_point) {
				result.draw(s, higher_point, finish);
			}
		}
	} else if (!is_drawing_inside)
		result.draw(s, start, finish);
}

void draw_figure(SDL_Surface *s, Point* points, Point* section_window, 
				 int side_count, bool draw_inside, Uint32 colour) {
	for (int i = 0; i < side_count; i++) {
		if (USE_SECTION_WINDOW) 
			cyrus_beck_line(s, points[i], points[NEXT(i, side_count)], section_window, draw_inside, colour);
		else {
			WuLine l = WuLine(Colour(colour));
			l.draw(s, points[i], points[NEXT(i, side_count)]);
		}
	}
}

void recount_coordinates(Point* coordinates, int side_count, float side_separator_coef) {
	Point *new_coordinates = (Point*)calloc(side_count, sizeof(Point));
	for (int i = 0; i < side_count; i++) {
		int current = i;
		int next = NEXT(i, side_count);
		new_coordinates[next].x = coordinates[current].x + 
			(coordinates[next].x - coordinates[current].x) * side_separator_coef;
		new_coordinates[next].y = coordinates[current].y + 
			(coordinates[next].y - coordinates[current].y) * side_separator_coef;
	}
	for (int i = 0; i < side_count; i++) {
		coordinates[i].x = new_coordinates[i].x;
		coordinates[i].y = new_coordinates[i].y;
	}
	free(new_coordinates);
}

void draw_section_window(SDL_Surface *s, Point* points, int side_count, Uint32 colour) {
	for (int i = 0; i < side_count; i++) {
		WuLine line = WuLine(Colour(colour));
		line.draw(s, points[i], points[NEXT(i, side_count)]);
	}
}

Point recount_point(Point point, Matrix mtrx_final) {
	std::vector<double> counted_coordinates;
	counted_coordinates.push_back(point.x);
	counted_coordinates.push_back(point.y);
	counted_coordinates.push_back(1);
	std::vector<double> result = mtrx_final * counted_coordinates;
	Point new_point = Point(result[0], result[1]);
	return new_point;
}

void draw(SDL_Surface *s, Matrix mtrx_finals[DRAWN_FIGURES_COUNT], bool draw_inside)
{
	sect_wnd_side_count = figures_side_count[0];

	int sect_wnd_used = USE_SECTION_WINDOW ? 1 : 0;
	Point *section_window = NULL;
	if (USE_SECTION_WINDOW) {
		section_window = (Point*)calloc(sect_wnd_side_count, sizeof(Point));
		int sect_wnd_radius = RADIUS * 2;
		init_start_coordinates(section_window, sect_wnd_side_count, sect_wnd_radius, mtrx_finals[0]);
		draw_section_window(s, section_window, sect_wnd_side_count, colours[0]);
	}

	Point **coordinates = (Point**)calloc(DRAWN_FIGURES_COUNT - sect_wnd_used, sizeof(Point*));
	for (int i = 0; i < DRAWN_FIGURES_COUNT - sect_wnd_used; i++) {
		coordinates[i] = (Point*)calloc(figures_side_count[i + sect_wnd_used], sizeof(Point));
		init_start_coordinates(coordinates[i], figures_side_count[i + sect_wnd_used], RADIUS, mtrx_finals[i + sect_wnd_used]);
	}

	for (int i = 0; i < DRAWN_FIGURES_COUNT - sect_wnd_used; i++) {
		float tmp_tan = tan(M_PI / 4 * MULTIPLICITY / nested_figure_count[i + sect_wnd_used]);
		float side_separator_coef = tmp_tan / (tmp_tan + 1);
		for (int j = 0; j < nested_figure_count[i + sect_wnd_used]; j++) {
			draw_figure(s, coordinates[i], section_window, figures_side_count[i + sect_wnd_used], draw_inside, colours[i + sect_wnd_used]);
			recount_coordinates(coordinates[i], figures_side_count[i + sect_wnd_used], side_separator_coef);
		}
	}

	for (int i = 0; i < DRAWN_FIGURES_COUNT - sect_wnd_used; i++)
		free(coordinates[i]);
	free(coordinates);

	if (USE_SECTION_WINDOW)
		free(section_window);
}