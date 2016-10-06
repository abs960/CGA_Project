#include "draw.h"
#include "stdafx.h"
#include "math.h"
#include "time.h"

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

Point get_new_coordinate(Point point, Matrix mtrxF);
Uint32 change_colour(Uint32 base, float intensity);

const float LITTLE_ANGLE = 5,
			BIG_ANGLE = 180 - LITTLE_ANGLE,
			STEP = 0.05;

const int	MULTIPLICITY	= 3,
			COLOUR_GREEN	= 0x0000FF00,
			COLOUR_RED		= 0x00FF0000;

extern int	side_count;

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	assert(NULL != surface);
	assert(x < SCREEN_WIDTH);
	assert(y < SCREEN_HEIGHT);

	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = pixel;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
	assert(NULL != surface);
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

void init_start_coordinates(Point* start, int side_count, int radius, Matrix mtrxF) {
	int start_angle = abs(90 - (360 / side_count) / 2);
	for (int i = 0; i < side_count; i++) {
		Point tmp;
		tmp.x = radius * cos(2 * M_PI * i / side_count + RAD(start_angle));
		tmp.y = radius * sin(2 * M_PI * i / side_count + RAD(start_angle));
		start[i] = get_new_coordinate(tmp, mtrxF);
	}
}

void swap(float *first, float *second) {
	float tmp = *first;
	*first = *second;
	*second = tmp;
}

void draw_wu_point(bool more_vertical, SDL_Surface *s, int x, int y, Matrix mtrxF, float intensity, int colour) {
	Uint32 new_colour = change_colour(colour, intensity);
	if (more_vertical) 
		put_pixel32(s, y, x, new_colour);
	else 
		put_pixel32(s, x, y, new_colour);
}

void wu_line(SDL_Surface *s, Point first, Point second, Matrix mtrxF, int colour) {
	bool more_vertical = abs(second.y - first.y) > abs(second.x - first.x);
	if (more_vertical) {
		swap(&first.x, &first.y);
		swap(&second.x, &second.y);
	}
	if (first.x > second.x) {
		swap(&first.x, &second.x);
		swap(&first.y, &second.y);
	}

	draw_wu_point(more_vertical, s, first.x, first.y, mtrxF, 1, colour);
	draw_wu_point(more_vertical, s, second.x, second.y, mtrxF, 1, colour);
	float dx = second.x - first.x;
	float dy = second.y - first.y;
	float k = dy / dx;
	float y = first.y + k;
	for (float x = first.x + 1; x <= second.x - 1; x++) {
		int y_int = floor(y);
		float intensity = y - y_int;
		if (intensity == 0)
			intensity = 1;
		draw_wu_point(more_vertical, s, x, y_int, mtrxF, 1 - intensity, colour);
		draw_wu_point(more_vertical, s, x, y_int + 1, mtrxF, intensity, colour);
		y += k;
	}
}

void brezenheim_line(SDL_Surface *s, Point first, Point second, Matrix mtrxF, int colour) {
	int delta_x = abs(second.x - first.x);
	int delta_y = abs(second.y - first.y);
	int step_x = second.x >= first.x ? 1 : -1;
	int step_y = second.y >= first.y ? 1 : -1;
	if (delta_y <= delta_x) {
		int d = (delta_y << 1) - delta_x; 
		int d1 = delta_y << 1;
		int d2 = (delta_y - delta_x) << 1;
		put_pixel32(s, first.x, first.y, colour);
		for (int x = first.x + step_x, y = first.y, i = 1; i <= delta_x; i++, x += step_x) {
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
		put_pixel32(s, first.x, first.y, colour);
		for (int x = first.x, y = first.y + step_y, i = 1; i <= delta_y; i++, y += step_y) {
			if (d > 0) {
				d += d2; 
				x += step_x;
			} else
				d += d1;
			put_pixel32(s, x, y, colour);
		}
	}
}

void draw_figure(SDL_Surface *s, Point* points, int side_count, Matrix mtrxF) {
	int colour = COLOUR_GREEN;
	for (int i = 0; i < side_count; i++) {
		wu_line(s, points[i], points[(i + 1) % side_count], mtrxF, colour);
	}
}

void recount_coordinates(Point* coordinates, int side_count, float side_separator_coef) {
	Point *newCoordinates = (Point*)calloc(side_count, sizeof(Point));
	for (int i = 0; i < side_count; i++) {
		int current = i % side_count;
		int next = (i + 1) % side_count;
		newCoordinates[next].x = coordinates[current].x + 
			(coordinates[next].x - coordinates[current].x) * side_separator_coef;
		newCoordinates[next].y = coordinates[current].y + 
			(coordinates[next].y - coordinates[current].y) * side_separator_coef;
	}
	for (int i = 0; i < side_count; i++) {
		coordinates[i].x = newCoordinates[i].x;
		coordinates[i].y = newCoordinates[i].y;
	}
	free(newCoordinates);
}

void draw_section_window(SDL_Surface *s, Point* points, int side_count, Matrix mtrxF) {
	int colour = COLOUR_RED;
	for (int i = 0; i < side_count; i++) {
		wu_line(s, points[i], points[(i + 1) % side_count], mtrxF, colour);
	}
}

int generate_colour() {
	int colour = 0;
	do {
		srand(time(NULL));
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		colour = RGB(r, g, b);
	} while (colour == 0);
	return colour;
}

Point get_new_coordinate(Point point, Matrix mtrxF) {
	float *result = (float*)calloc(3, sizeof(float));
	float counted_coordinates[3] = { point.x, point.y, 1 };
	mult_mtrx_vector(mtrxF, counted_coordinates, result);

	Point new_point;
	new_point.x = result[0];
	new_point.y = result[1];

	free(result);
	return new_point;
}

Uint32 change_colour(Uint32 base, float intensity) {
	int b = (base & 0xFF) * intensity;
	int g = ((base & 0xFF00) >> 8) * intensity;
	int r = ((base & 0xFF0000) >> 10) * intensity;
	return RGB32(r, g, b);
}

void draw(SDL_Surface *s, Matrix mtrxF[DRAWN_FIGURES_COUNT], int nested_figure_count, bool draw_inside)
{
	Point *section_window = (Point*)calloc(4, sizeof(Point));
	init_start_coordinates(section_window, 4, RADIUS, mtrxF[0]);
	draw_section_window(s, section_window, 4, mtrxF[0]);

	Point **coordinates = (Point**)calloc(DRAWN_FIGURES_COUNT - 1, sizeof(Point*));
	for (int i = 0; i < DRAWN_FIGURES_COUNT - 1; i++) {
		coordinates[i] = (Point*)calloc(side_count, sizeof(Point));
		init_start_coordinates(coordinates[i], side_count, RADIUS, mtrxF[i + 1]);
	}

	float tmp_tan = tan(M_PI / 4 * MULTIPLICITY / nested_figure_count);
	float side_separator_coef = tmp_tan / (tmp_tan + 1);

	for (int i = 0; i < DRAWN_FIGURES_COUNT - 1; i++)
		for (int j = 0; j < nested_figure_count; j++) {
			draw_figure(s, coordinates[i], side_count, mtrxF[i + 1]);
			recount_coordinates(coordinates[i], side_count, side_separator_coef);
		}

	for (int i = 0; i < DRAWN_FIGURES_COUNT - 1; i++)
		free(coordinates[i]);
	free(coordinates);

	free(section_window);
}