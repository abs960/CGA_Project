#include "Facet.h"

Facet::Facet() {}

Facet::Facet(const Facet & copy) {
	points = copy.points;
}

Facet::~Facet() {}

void Facet::add_point(Point point) {
	points.push_back(point);
}

Point Facet::get_point(int count) {
	return points.at(count);
}

int Facet::get_point_count() {
	return points.size();
}

void Facet::recount(Matrix * matrix) {
	for (int i = 0; i < points.size(); i++) {
		std::vector<double> v;
		v.push_back(points[i].x);
		v.push_back(points[i].y);
		v.push_back(points[i].z);
		v.push_back(1);
		std::vector<double> res = (*matrix) * v;
		points[i].x = res.at(0);
		points[i].y = res.at(1);
		points[i].z = res.at(2);
	}
}

void Facet::recount(Quaternion q) {
	for (int i = 0; i < points.size(); i++) {
		Point point = points.at(i);
		Point tmp = q.transform(point);
		points[i] = tmp;
	}
}

void Facet::recount(Matrix * matrix, Quaternion q) {
	for (int i = 0; i < points.size(); i++) {
		Point point = points.at(i);
		Point tmp = q.transform(point);

		std::vector<double> v;
		v.push_back(tmp.x);
		v.push_back(tmp.y);
		v.push_back(tmp.z);
		v.push_back(1);
		std::vector<double> res = (*matrix) * v;
		points[i].x = res.at(0);
		points[i].y = res.at(1);
		points[i].z = res.at(2);
	}
}

void Facet::apply_perspective(Point projection_center, Vector angles) {
	for (int i = 0; i < points.size(); i++) {
		Point cur = points.at(i);

		double d = 1 + (cur.z) / (projection_center.z);
		points[i] = Point(
			cur.x * d, 
			cur.y * d,
			cur.z
		);
	}
}

Point Facet::get_closest_point() {
	float min_z = INFINITY;
	int min_z_count = -1;
	for (int i = 0; i < points.size(); i++) {
		if (points.at(i).z < min_z) {
			min_z = points.at(i).z;
			min_z_count = i;
		}
	}
	return points.at(min_z_count);
}

bool Facet::is_visible(Vector observer) {
	Vector normal = get_normal();
	double scalar_mult_res = normal * observer;
	return scalar_mult_res > 0;
}

Vector Facet::get_normal() {
	int first = 0;
	int second = NEXT(first, points.size());
	int third = NEXT(second, points.size());

	Vector edge0 = points.at(first) - points.at(second);
	Vector edge1 = points.at(second) - points.at(third);
	Vector normal = edge0.vector_mult(edge1);
	return normal;
}

Point Facet::left() {
	int x_min = 0;
	for (int j = 0; j < points.size(); j++) {
		if (points.at(j).x < points.at(x_min).x) {
			x_min = j;
		}
	}
	return points.at(x_min);
}

Point Facet::right() {
	int x_max = 0;
	for (int j = 0; j < points.size(); j++) {
		if (points.at(j).x > points.at(x_max).x) {
			x_max = j;
		}
	}
	return points.at(x_max);
}

Point Facet::top() {
	int y_min = 0;
	for (int j = 0; j < points.size(); j++) {
		if (points.at(j).y < points.at(y_min).y) {
			y_min = j;
		}
	}
	return points.at(y_min);
}

Point Facet::bottom() {
	int y_max = 0;
	for (int j = 0; j < points.size(); j++) {
		if (points.at(j).y > points.at(y_max).y) {
			y_max = j;
		}
	}
	return points.at(y_max);
}

bool Facet::contains(Point p) {
	p.z = 0;
	bool res = true;
	int size = points.size();
	for (int i = 0; i < size; i++) {
		Point start = points.at(i);
		start.z = 0;
		Point finish = points.at(NEXT(i, size));
		finish.z = 0;
		Vector normal = Point(start.y - finish.y, finish.x - start.x, 0);
		Vector w = p - start;
		double ws = w * normal;
		if (ws < 0) {
			res = false;
			break;
		}
	}
	return res;
}

std::vector<int> Facet::get_x_borders(int y) {
	std::vector<int> x_borders;

	int size = points.size();
	for (int i = 0; i < size; i++) {
		// check if horizontal
		if (abs(points.at(i).y - points.at(NEXT(i, size)).y) < ALMOST_ZERO) {
			continue;
		}

		Point y_min, y_max;
		if (points.at(i).y > points.at(NEXT(i, size)).y) {
			y_max = points.at(i);
			y_min = points.at(NEXT(i, size));
		} else {
			y_min = points.at(i);
			y_max = points.at(NEXT(i, size));
		}

		// check if given y coordinate belongs to current side
		if (y >= y_min.y && y <= y_max.y) {
			if (y == y_min.y) {
				x_borders.push_back(y_min.x);
				continue;
			}
			if (y == y_max.y) {
				x_borders.push_back(y_max.x);
				continue;
			}
			float dy = y - y_min.y;
			float dy_total = y_max.y - y_min.y;
			float k = dy / dy_total;
			int x = y_min.x + (y_max.x - y_min.x) * k;
			x_borders.push_back(x);
		}
	}

	return x_borders;
}

void Facet::fill(SDL_Surface * s, Colour colour, Vector observer) {
	Vector normal = get_normal();
	double scal_mult = observer * normal;
	double mod_mult = observer.modulus() * normal.modulus();

	double intensity = scal_mult / mod_mult;
	Colour c = Colour(colour.get_value() * 0.75);
	c.change_intensity(intensity);

	int y_top = top().y;
	int y_bottom = bottom().y;
	for (int y = y_top; y < y_bottom; y++) {
		std::vector<int> x_borders = get_x_borders(y);
		
		int size = x_borders.size();
		if (size <= 1) {
			continue;
		}
		for (int i = 0; i < size - 1; i+=2) {
			int x_start = x_borders.at(i);
			int x_finish = x_borders.at(NEXT(i, size));
			if (x_start > x_finish) {
				int tmp = x_start;
				x_start = x_finish;
				x_finish = tmp;
			}
			for (int x = x_start; x < x_finish; x++) {
				if (get_pixel32(s, x, y) == Colour::COLOUR_BLACK)
					put_pixel32(s, x, y, c.get_value());
			}
		}
	}
}

void Facet::draw(SDL_Surface * s, Line* brush) {
	for (int i = 0; i < points.size() - 1; i++)
		brush->draw(s, points.at(i), points.at(i + 1));
}

Facet & Facet::operator=(const Facet & other) {
	if (&other == this)
		return *this;

	points = other.points;

	return *this;
}