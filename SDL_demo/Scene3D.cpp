#include "Scene3D.h"

Scene3D::Scene3D() {
	colour = Colour();
	matrix = new Matrix(4);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			double element = i == j ? 1 : 0;
			matrix->set_element(i, j, element);
		}

	store_shift = Point(SCREEN_WIDTH / 2 - SIDE_LENGTH, SCREEN_HEIGHT / 2 + SIDE_LENGTH, 0);
	matrix->set_element(0, 3, store_shift.x);
	matrix->set_element(1, 3, store_shift.y);
	matrix->set_element(2, 3, store_shift.z);
}

Scene3D::Scene3D(const Scene3D & copy) {
	colour = copy.colour;
	*base_line = *(copy.base_line);
	objects = copy.objects;
	*matrix = *(copy.matrix);
}

Scene3D::~Scene3D() {
	delete matrix;
	if (base_line != nullptr)
		delete base_line;
}

void Scene3D::add_object(Shape3D * object) {
	objects.push_back(object);
}

Shape3D * Scene3D::get_object(int count) {
	return objects.at(count);
}

void Scene3D::clear_scene() {
	objects.clear();
}

int Scene3D::get_objects_count() {
	return objects.size();
}

void Scene3D::set_base_line(Line * line) {
	base_line = line;
	base_line->set_colour(colour);
}

void Scene3D::set_colour(Colour colour) {
	this->colour = colour;
}

void Scene3D::rotate_x(float d_angle) {
	save_shift();

	float rad_angle = RAD(d_angle);
	Matrix tmp = Matrix(4);
	tmp.set_element(0, 0, 1);
	tmp.set_element(1, 1, cos(rad_angle));
	tmp.set_element(1, 2, -sin(rad_angle));
	tmp.set_element(2, 1, sin(rad_angle));
	tmp.set_element(2, 2, cos(rad_angle));
	tmp.set_element(3, 3, 1);
	*matrix *= tmp;

	load_shift();
}

void Scene3D::rotate_y(float d_angle) {
	save_shift();

	float rad_angle = RAD(d_angle);
	Matrix tmp = Matrix(4);
	tmp.set_element(0, 0, cos(rad_angle));
	tmp.set_element(0, 2, sin(rad_angle));
	tmp.set_element(1, 1, 1);
	tmp.set_element(2, 0, -sin(rad_angle));
	tmp.set_element(2, 2, cos(rad_angle));
	tmp.set_element(3, 3, 1);
	*matrix *= tmp;

	load_shift();
}

void Scene3D::rotate_z(float d_angle) {
	save_shift();

	float rad_angle = RAD(d_angle);
	Matrix tmp = Matrix(4);
	tmp.set_element(0, 0, cos(rad_angle));
	tmp.set_element(0, 1, -sin(rad_angle));
	tmp.set_element(1, 0, sin(rad_angle));
	tmp.set_element(1, 1, cos(rad_angle));
	tmp.set_element(2, 2, 1);
	tmp.set_element(3, 3, 1);
	*matrix *= tmp;

	load_shift();
}

void Scene3D::move(float dx, float dy, float dz) {
	Matrix tmp = Matrix(4);
	tmp.set_element(0, 0, 1);
	tmp.set_element(1, 1, 1);
	tmp.set_element(2, 2, 1);
	tmp.set_element(3, 3, 1);
	tmp.set_element(0, 3, dx);
	tmp.set_element(1, 3, dy);
	tmp.set_element(2, 3, dz);
	*matrix *= tmp;
}

void Scene3D::scale(float d_scale) {
	save_shift();

	Matrix tmp = Matrix(4);
	tmp.set_element(0, 0, d_scale);
	tmp.set_element(1, 1, d_scale);
	tmp.set_element(2, 2, d_scale);
	tmp.set_element(3, 3, 1);
	*matrix *= tmp;

	load_shift();
}

void Scene3D::draw(SDL_Surface * s) {
	for (int i = 0; i < objects.size(); i++)
		objects.at(i)->recount(matrix);

	std::vector<Facet> visible_facets;
	for (int i = 0; i < objects.size(); i++) {
		std::vector<Facet> object_visible_facets = objects.at(i)->get_visible_facets(observer);
		for (int i = 0; i < object_visible_facets.size(); i++)
			visible_facets.push_back(object_visible_facets.at(i));
	}

	std::sort(visible_facets.begin(), visible_facets.end(), [this](Facet facet0, Facet facet1) {
		Point closest_point0 = facet0.get_closest_point();
		Point closest_point1 = facet1.get_closest_point();
		return closest_point0.z > closest_point1.z;
	});

	for (int i = 0; i < visible_facets.size(); i++) {
		Facet current_facet = visible_facets.at(i);
		int side_count = current_facet.get_side_count();
		for (int facet_point_count = 0; facet_point_count < side_count; facet_point_count++) {
			Point side_start = current_facet.get_point(facet_point_count);
			Point side_finish = current_facet.get_point(NEXT(facet_point_count, side_count));

			Point directrix = side_finish - side_start;
			double t, t_low = 1, t_high = 0;
			boolean is_entirely_visible = true;
			for (int k = 0; k < i; k++) {
				Facet sw_facet = visible_facets.at(k);
				double tmp_t_low = 0, tmp_t_high = 1;
				boolean is_entirely_visible_tmp = false;

				int sw_side_count = sw_facet.get_side_count();
				for (int sw_facet_point_count = 0; sw_facet_point_count < sw_side_count; sw_facet_point_count++) {
					Point sw_side_start = sw_facet.get_point(sw_facet_point_count);
					Point sw_side_finish = sw_facet.get_point(NEXT(sw_facet_point_count, sw_side_count));

					Vector normal = right_normal(sw_side_start, sw_side_finish);
					Vector w = side_start - sw_side_start;
					double w_scalar = w * normal;
					double d_scalar = directrix * normal;
					if (abs(d_scalar) < ALMOST_ZERO) {
						if (w_scalar < -ALMOST_ZERO) {
							is_entirely_visible_tmp = true;
							break;
						}
						continue;
					}
					t = -w_scalar / d_scalar;
					if (d_scalar < -ALMOST_ZERO) {
						if (t <= ALMOST_ZERO) {
							is_entirely_visible_tmp = true;
							break;
						} else if (ALMOST_ZERO < t && t < 1 - ALMOST_ZERO) {
							is_entirely_visible_tmp = false;
							tmp_t_high = min(tmp_t_high, t);
						} else if (t >= 1) {
							is_entirely_visible_tmp = false;
							tmp_t_high = min(tmp_t_high, 1);
						}
					} else if (d_scalar > ALMOST_ZERO) {
						if (t <= ALMOST_ZERO) {
							is_entirely_visible_tmp = false;
							tmp_t_low = max(tmp_t_low, 0);
						} else if (ALMOST_ZERO < t && t < 1 - ALMOST_ZERO) {
							is_entirely_visible_tmp = false;
							tmp_t_low = max(tmp_t_low, t);
						} else if (t >= 1 - ALMOST_ZERO) {
							is_entirely_visible_tmp = true;
							break;
						}
					}
				}
				if (tmp_t_low > tmp_t_high)
					is_entirely_visible_tmp = true;

				is_entirely_visible &= is_entirely_visible_tmp;

				if (!is_entirely_visible_tmp) {
					t_low = min(t_low, tmp_t_low);
					t_high = max(t_high, tmp_t_high);
				}
			}
			if (is_entirely_visible) {
				base_line->draw(s, side_start, side_finish);
			} else if (t_low < t_high) {
				Point low_point = side_start + (side_finish - side_start) * t_low;
				Point high_point = side_start + (side_finish - side_start) * t_high;
				if (side_start != low_point)
					base_line->draw(s, side_start, low_point);
				if (high_point != side_finish)
					base_line->draw(s, high_point, side_finish);
			}
		}
	}
}

Scene3D & Scene3D::operator=(const Scene3D & other) {
	if (&other == this)
		return *this;

	colour = other.colour;
	*base_line = *(other.base_line);
	objects = other.objects;
	*matrix = *(other.matrix);

	return *this;
}

Point Scene3D::left_normal(Point p0, Point p1) {
	return Point(p0.y - p1.y, p1.x - p0.x, 0);
}

Point Scene3D::right_normal(Point p0, Point p1) {
	return Point(p1.y - p0.y, p0.x - p1.x, 0);
}

void Scene3D::save_shift() {
	double x = matrix->get_element(0, 3);
	double y = matrix->get_element(1, 3);
	double z = matrix->get_element(2, 3);
	matrix->set_element(0, 3, 0);
	matrix->set_element(1, 3, 0);
	matrix->set_element(2, 3, 0);

	store_shift = Point(x, y, z);
}

void Scene3D::load_shift() {
	matrix->set_element(0, 3, store_shift.x);
	matrix->set_element(1, 3, store_shift.y);
	matrix->set_element(2, 3, store_shift.z);
}