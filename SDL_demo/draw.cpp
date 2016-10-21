#include "draw.h"
#include "stdafx.h"
#include "math.h"

Point recount_point(Point point, Matrix mtrxF);
Uint32 change_colour(Uint32 base, float intensity);
Uint32 change_colour_ycbcr(Uint32 base, float intensity);

const int	MULTIPLICITY = 3;

// Extern global variables
int			figures_side_count[DRAWN_FIGURES_COUNT] = { 4 };
int			nested_figure_count[DRAWN_FIGURES_COUNT] = { 9 };
Uint32		colours[DRAWN_FIGURES_COUNT] = { 0x0000FF00 };

int			sect_wnd_side_count;

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 colour)
{
	assert(NULL != surface);
	/*assert(x > 0);
	assert(y > 0);
	assert(x < SCREEN_WIDTH);
	assert(y < SCREEN_HEIGHT);*/
	if (x < 0 || x > SCREEN_WIDTH ||
		y < 0 || y > SCREEN_HEIGHT)
		return;

	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = colour;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
	assert(NULL != surface);
	assert(x > 0);
	assert(y > 0);
	assert(x < SCREEN_WIDTH);
	assert(y < SCREEN_HEIGHT);

	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}

void mult_mtrx(Matrix left, Matrix right, Matrix result) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			result[i][j] = 0;
			for (int k = 0; k < 3; k++)
				result[i][j] += left[i][k] * right[k][j];
		}
}

void mult_mtrx_vector(Matrix mtrx, float* vector, float* result) {
	for (int i = 0; i < 3; i++) {
		result[i] = 0;
		for (int j = 0; j < 3; j++)
			result[i] += mtrx[i][j] * vector[j];
	}
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

void swap(float *first, float *second) {
	float tmp = *first;
	*first = *second;
	*second = tmp;
}

void draw_wu_point(bool more_vertical, SDL_Surface *s, int x, int y, float intensity, Uint32 colour) {
	Uint32 new_colour = change_colour(colour, intensity);
	if (more_vertical) 
		put_pixel32(s, y, x, new_colour);
	else 
		put_pixel32(s, x, y, new_colour);
}

void wu_line(SDL_Surface *s, Line line, Uint32 colour) {
	bool more_vertical = abs(line.finish.y - line.start.y) > abs(line.finish.x - line.start.x);
	if (more_vertical) {
		swap(&line.start.x, &line.start.y);
		swap(&line.finish.x, &line.finish.y);
	}
	if (line.start.x > line.finish.x) {
		swap(&line.start.x, &line.finish.x);
		swap(&line.start.y, &line.finish.y);
	}

	draw_wu_point(more_vertical, s, line.start.x, line.start.y, 1, colour);
	draw_wu_point(more_vertical, s, line.finish.x, line.finish.y, 1, colour);
	float dx = line.finish.x - line.start.x;
	float dy = line.finish.y - line.start.y;
	float k = dy / dx;
	float y = line.start.y + k;
	for (int x = line.start.x + 1; x <= line.finish.x - 1; x++) {
		int y_int = floor(y);
		float intensity = y - y_int;
		draw_wu_point(more_vertical, s, x, y_int, 1 - intensity, colour);
		draw_wu_point(more_vertical, s, x, y_int + 1, intensity, colour);
		y += k;
	}
}

void brezenheim_line(SDL_Surface *s, Line line, Uint32 colour) {
	int delta_x = abs(line.finish.x - line.start.x);
	int delta_y = abs(line.finish.y - line.start.y);
	int step_x = line.finish.x >= line.start.x ? 1 : -1;
	int step_y = line.finish.y >= line.start.y ? 1 : -1;
	if (delta_y <= delta_x) {
		int d = (delta_y << 1) - delta_x; 
		int d1 = delta_y << 1;
		int d2 = (delta_y - delta_x) << 1;
		put_pixel32(s, line.start.x, line.start.y, colour);
		for (int x = line.start.x + step_x, y = line.start.y, i = 1; i <= delta_x; i++, x += step_x) {
			if (d > 0) {
				d += d2; 
				y += step_y;
			} else d += d1;
			put_pixel32(s, x, y, colour);
		}
	} else {
		int d = (delta_x << 1) - delta_y; 
		int d1 = delta_x << 1;
		int d2 = (delta_x - delta_y) << 1;
		put_pixel32(s, line.start.x, line.start.y, colour);
		for (int x = line.start.x, y = line.start.y + step_y, i = 1; i <= delta_y; i++, y += step_y) {
			if (d > 0) {
				d += d2; 
				x += step_x;
			} else
				d += d1;
			put_pixel32(s, x, y, colour);
		}
	}
}

float scalar_mult(Point p1, Point p2) {
	return p1.x * p2.x + p1.y * p2.y;
}

void cyrus_beck_line(SDL_Surface *s, Line line, Point *section_window, bool is_drawing_inside, Uint32 colour) {
	float t_low = 0, t_high = 1, t;
	Point directrix = line.finish - line.start;
	for (int i = 0; i < sect_wnd_side_count; i++) {
		Line sect_wnd_side = Line(section_window[i], section_window[NEXT(i, sect_wnd_side_count)]);
		Point normal = Point(sect_wnd_side.start.y - sect_wnd_side.finish.y, 
							 sect_wnd_side.finish.x - sect_wnd_side.start.x);
		Point sect_wnd_side_point = sect_wnd_side.start;
		Point w = line.start - sect_wnd_side_point;
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
		Point lower_point = line.start + (line.finish - line.start) * t_low;
		Point higher_point = line.start + (line.finish - line.start) * t_high;
		Line result;
		if (is_drawing_inside) {
			result = Line(lower_point, higher_point);
			wu_line(s, result, colour);
		} else {
			if (line.start != lower_point) {
				result = Line(line.start, lower_point);
				wu_line(s, result, colour);
			}
			if (line.finish != higher_point) {
				result = Line(higher_point, line.finish);
				wu_line(s, result, colour);
			}
		}
	} else if (!is_drawing_inside)
		wu_line(s, line, colour);
}

void draw_figure(SDL_Surface *s, Point* points, Point* section_window, 
				 int side_count, bool draw_inside, Uint32 colour) {
	for (int i = 0; i < side_count; i++) {
		Line figure_side = Line(points[i], points[NEXT(i, side_count)]);

		if (USE_SECTION_WINDOW) 
			cyrus_beck_line(s, figure_side, section_window, draw_inside, colour);
		else 
			wu_line(s, figure_side, colour);
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
		Line line = Line(points[i], points[NEXT(i, side_count)]);
		wu_line(s, line, colour);
	}
}

Point recount_point(Point point, Matrix mtrx_final) {
	float *result = (float*)calloc(3, sizeof(float));
	float counted_coordinates[3] = { point.x, point.y, 1 };
	mult_mtrx_vector(mtrx_final, counted_coordinates, result);
	Point new_point = Point(result[0], result[1]);
	free(result);
	return new_point;
}

Uint32 change_colour_ycbcr(Uint32 base, float intensity) {
	if (intensity == 0)
		return 0;
	// Getting original RGB components
	int r = ((base & 0xFF0000) >> 10);
	int g = ((base & 0xFF00) >> 8);
	int b = (base & 0xFF);
	// Converting RGB to YCbCr
	float y = 0.299 * r + 0.587 * g + 0.114 * b;/*
	float cb = 128 - 0.168736 * r - 0.331264 * g + 0.5 * b;
	float cr = 128 + 0.5 * r - 0.418688 * g - 0.081312 * b;*/
	float cb = 0;
	float cr = 0;
	// Modifying the intensity
	y *= intensity;
	// Converting back to RGB
	r = y + 1.402 * (cr - 128);
	g = y - 0.344136 * (cb - 128) - 0.714136 * (cr - 128);
	b = y + 1.772 * (cb - 128);
	return RGB32(r, g, b);
}

Uint32 change_colour(Uint32 base, float intensity) {
	int r = ((base & 0xFF0000) >> 10) * intensity;
	int g = ((base & 0xFF00) >> 8) * intensity;
	int b = (base & 0xFF) * intensity;
	return RGB32(r, g, b);
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