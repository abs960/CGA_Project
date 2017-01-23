#include "Scene2D.h"

Scene2D::Scene2D() {
	matrix = new Matrix(3);
	clear_matrix();
}

Scene2D::Scene2D(const Scene2D & copy) {
	surface = copy.surface;
	*background_colour = *(copy.background_colour);
	*matrix = *(copy.matrix);
	clear_objects();
	objects = copy.objects;
}

Scene2D::~Scene2D() {
	clear_objects();
}

void Scene2D::add_object(MatrixShape2D * s) {
	objects.push_back(s);
}

MatrixShape2D * Scene2D::get_object(int pos) {
	if (pos < 0 || pos >= objects.size())
		return nullptr;
	return objects.at(pos);
}

void Scene2D::remove_object(int pos) {
	if (pos < 0 || pos >= objects.size())
		return;
	objects.erase(objects.begin() + pos);
}

void Scene2D::clear_scene() {
	clear_objects();
}

int Scene2D::get_objects_count() {
	return objects.size();
}

void Scene2D::rotate(float d_angle) {
	rotate_z(d_angle);
}

void Scene2D::rotate_x(float d_angle) {}

void Scene2D::rotate_y(float d_angle) {}

void Scene2D::rotate_z(float d_angle) {
	float rad_angle = RAD(d_angle);
	Matrix tmp = Matrix(3);
	tmp.set_element(0, 0, cos(rad_angle));
	tmp.set_element(0, 1, -sin(rad_angle));
	tmp.set_element(1, 0, sin(rad_angle));
	tmp.set_element(1, 1, cos(rad_angle));
	tmp.set_element(2, 2, 1);
	*matrix *= tmp;
	apply_matrix(tmp);
}

void Scene2D::rotate_vector(Vector v, float d_angle) {}

void Scene2D::move(float dx, float dy, float dz) {
	Matrix tmp = Matrix(3);
	tmp.set_element(0, 0, 1);
	tmp.set_element(0, 2, dx);
	tmp.set_element(1, 1, 1);
	tmp.set_element(1, 2, dy);
	tmp.set_element(2, 2, 1);
	*matrix *= tmp;
	apply_matrix(tmp);
}

void Scene2D::scale(float d_scale) {
	Matrix tmp = Matrix(3);
	tmp.set_element(0, 0, 1 + d_scale);
	tmp.set_element(1, 1, 1 + d_scale);
	tmp.set_element(2, 2, 1);
	*matrix *= tmp;
	apply_matrix(tmp);
}

void Scene2D::draw() {
	SDL_FillRect(surface, NULL, background_colour->get_value());
	for (MatrixShape2D* s : objects) {
		s->draw(surface);
	}
}

Scene2D & Scene2D::operator=(const Scene2D & other) {
	if (&other == this)
		return *this;

	surface = other.surface;
	*background_colour = *(other.background_colour);
	*matrix = *(other.matrix);
	clear_objects();
	objects = other.objects;

	return *this;
}

void Scene2D::clear_objects() {
	for (MatrixShape2D* s : objects) {
		delete s;
	}
	objects.clear();
}

void Scene2D::clear_matrix() {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			matrix->set_element(i, j, (i == j ? 1 : 0));
}

void Scene2D::apply_matrix(Matrix m) {
	for (MatrixShape2D* s : objects) {
		s->mult_by_matrix(&m);
	}
}
