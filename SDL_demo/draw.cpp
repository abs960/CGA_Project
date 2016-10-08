#include "draw.h"
#include "stdafx.h"
#include "math.h"

Point get_new_coordinate(Point point, Matrix mtrxF);
Uint32 change_colour(Uint32 base, float intensity);

const float LITTLE_ANGLE = 5,
			BIG_ANGLE = 180 - LITTLE_ANGLE,
			STEP = 0.05;

const int	MULTIPLICITY = 3;

int			side_count[DRAWN_FIGURES_COUNT] = { 4 };
int			nested_figure_count[DRAWN_FIGURES_COUNT] = { 9 };
Uint32		colours[DRAWN_FIGURES_COUNT] = { 0x0000FF00 };

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

void init_start_coordinates(Point* start, int side_count, int radius, Matrix mtrxF) {
	int start_angle = abs(90 - (360 / side_count) / 2);
	for (int i = 0; i < side_count; i++) {
		Point tmp;
		tmp.x = radius * cos(2 * M_PI * i / side_count + RAD(start_angle));
		tmp.y = radius * sin(2 * M_PI * i / side_count + RAD(start_angle));
		start[i] = get_new_coordinate(tmp, mtrxF);
	}
}

void swap(int *first, int *second) {
	int tmp = *first;
	*first = *second;
	*second = tmp;
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

void get_line_equation(Line *line) {
	line->x_coef = line->start.y - line->finish.y;
	line->y_coef = line->finish.x - line->start.x;
	line->no_coef = line->finish.x * line->start.y - line->start.x * line->finish.y;
}

bool get_lines_intersection(Line lines[2], Point *crossing) {
	for (int i = 0; i < 2; i++)
		get_line_equation(&(lines[i]));

	if (lines[0].x_coef * lines[1].y_coef == lines[1].x_coef * lines[0].y_coef)
		return false;

	crossing->x = -(lines[0].no_coef * lines[1].y_coef - lines[1].no_coef * lines[0].y_coef) / 
				(lines[0].x_coef * lines[1].y_coef - lines[1].x_coef * lines[0].y_coef);
	crossing->y = -(lines[0].x_coef * lines[1].no_coef - lines[1].x_coef * lines[0].no_coef) / 
				(lines[0].x_coef * lines[1].y_coef - lines[1].x_coef * lines[0].y_coef);

	return true;
}

bool point_inside_figure(Point point, Point* figure, int figure_side_count) {
	Line ray;
	ray.start = point;
	ray.finish.x = point.x + RADIUS * 2;
	ray.finish.y = point.y;

	Line side;
	int intersection_count = 0;
	for (int i = 0; i < figure_side_count; i++) {
		side.start = figure[i];
		side.finish = figure[(i + 1) % figure_side_count];
		Line lines[2] = { ray, side };
		Point intersection;
		intersection.x = -1;
		if (get_lines_intersection(lines, &intersection))
			intersection_count++;
	}
	if (intersection_count == 1)
		return true;
	else
		return false;
}

int line_length(Line line) {
	int dx = abs(line.start.x - line.finish.x);
	int dy = abs(line.start.y - line.finish.y);
	return sqrt(pow(dx, 2) + pow(dy, 2));
}

bool closer_to_start(Line line, Point point) {
	Line p_to_start;
	p_to_start.start = point;
	p_to_start.finish = line.start;
	Line p_to_finish;
	p_to_finish.start = point;
	p_to_finish.finish = line.finish;
	return line_length(p_to_start) > line_length(p_to_finish);
}

int get_intersections_with_figure(Line current, Point *figure, int side_count, Point *intersections) {
	int intersection_count = 0;
	for (int i = 0; i < side_count; i++) {
		Line side;
		side.start = figure[i];
		side.finish = figure[(i + 1) % side_count];
		Line lines[2] = { current, side };
		if (get_lines_intersection(lines, &(intersections[intersection_count])))
			intersection_count++;
		if (intersection_count == 2)
			break;
	}
	return intersection_count;
}

int get_lines_to_draw(Line current, Point* section_window, bool draw_inside, Line* result) {
	bool start_inside = point_inside_figure(current.start, section_window, side_count[0]);
	bool finish_inside = point_inside_figure(current.finish, section_window, side_count[0]);
	
	if (start_inside && finish_inside) {
		if (draw_inside) {
			result[0] = current;
			return 1;
		} else
			return 0;
	}

	Point *intersections = (Point*)calloc(2, sizeof(Point));
	int intersection_count = get_intersections_with_figure(current, section_window, side_count[0], intersections);

	if (intersection_count == 2) {
		if (draw_inside) {
			result[0].start = intersections[0];
			result[0].finish = intersections[1];
			return 1;
		} else {
			for (int i = 0; i < 2; i++) {
				if (closer_to_start(current, intersections[i])) {
					result[i].start = current.start;
					result[i].finish = intersections[i];
				} else {
					result[i].start = intersections[i];
					result[i].finish = current.finish;
				}
			}
			return 2;
		}
	}
	if (intersection_count == 1) {
		if (draw_inside) {
			if (start_inside) {
				result[0].start = current.start;
				result[0].finish = intersections[0];
			} else {
				result[0].start = intersections[0];
				result[0].finish = current.finish;
			}
		} else {
			if (start_inside) {
				result[0].start = intersections[0];
				result[0].finish = current.finish;
			} else {
				result[0].start = current.start;
				result[0].finish = intersections[0];
			}
		}
		return 1;
	}
	free(intersections);
	return 0;
}

void draw_figure(SDL_Surface *s, Point* points, Point* section_window, 
				 int side_count, bool draw_inside, Uint32 colour) {
	int lines_to_draw_count = 2;
	Line* lines_to_draw = (Line*)calloc(lines_to_draw_count, sizeof(Line));
	Line figure_side;
	for (int i = 0; i < side_count; i++) {
		figure_side.start = points[i];
		figure_side.finish = points[(i + 1) % side_count];

		if (USE_SECTION_WINDOW) {
			lines_to_draw_count = get_lines_to_draw(figure_side, section_window, draw_inside, lines_to_draw);
			for (int j = 0; j < lines_to_draw_count; j++)
				wu_line(s, lines_to_draw[i], colour);
		} else 
			wu_line(s, figure_side, colour);
	}
	free(lines_to_draw);
}

void recount_coordinates(Point* coordinates, int side_count, float side_separator_coef) {
	Point *new_coordinates = (Point*)calloc(side_count, sizeof(Point));
	for (int i = 0; i < side_count; i++) {
		int current = i % side_count;
		int next = (i + 1) % side_count;
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
		Line line;
		line.start = points[i];
		line.finish = points[(i + 1) % side_count];
		wu_line(s, line, colour);
	}
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

Uint32 change_colour_ycbcr(Uint32 base, float intensity) {
	if (intensity == 0)
		return 0;
	// Getting original RGB components
	int r = ((base & 0xFF0000) >> 10);
	int g = ((base & 0xFF00) >> 8);
	int b = (base & 0xFF);
	// Converting RGB to YCbCr
	float y = 0.299 * r + 0.587 * g + 0.114 * b;
	float cb = 128 - 0.168736 * r - 0.331264 * g + 0.5 * b;
	float cr = 128 + 0.5 * r - 0.418688 * g - 0.081312 * b;
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

void draw(SDL_Surface *s, Matrix mtrxF[DRAWN_FIGURES_COUNT], bool draw_inside)
{
	Point *section_window = (Point*)calloc(side_count[0], sizeof(Point));
	int section_window_radius = USE_SECTION_WINDOW ? RADIUS * 2 : RADIUS;
	init_start_coordinates(section_window, side_count[0], section_window_radius, mtrxF[0]);
	draw_section_window(s, section_window, side_count[0], colours[0]);

	Point **coordinates = (Point**)calloc(DRAWN_FIGURES_COUNT - 1, sizeof(Point*));
	for (int i = 0; i < DRAWN_FIGURES_COUNT - 1; i++) {
		coordinates[i] = (Point*)calloc(side_count[i + 1], sizeof(Point));
		init_start_coordinates(coordinates[i], side_count[i + 1], RADIUS, mtrxF[i + 1]);
	}

	for (int i = 0; i < DRAWN_FIGURES_COUNT - 1; i++) {
		float tmp_tan = tan(M_PI / 4 * MULTIPLICITY / nested_figure_count[i + 1]);
		float side_separator_coef = tmp_tan / (tmp_tan + 1);
		for (int j = 0; j < nested_figure_count[i + 1]; j++) {
			draw_figure(s, coordinates[i], section_window, side_count[i + 1], draw_inside, colours[i + 1]);
			recount_coordinates(coordinates[i], side_count[i + 1], side_separator_coef);
		}
	}

	for (int i = 0; i < DRAWN_FIGURES_COUNT - 1; i++)
		free(coordinates[i]);
	free(coordinates);

	free(section_window);
}