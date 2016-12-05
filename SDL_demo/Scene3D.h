#pragma once

#include "Shape3D.h"
#include "Line.h"
#include "Matrix.h"
#include "Colour.h"
#include "stdafx.h"

class Scene3D {
private:
	Point store_shift;

	void save_shift();
	void load_shift();
	void add_point_to_facet(Point p, Facet* f);
protected:
	Vector observer = Vector(0, 0, -1000);

	std::vector<Shape3D*> objects;
	Line* base_line;
	Matrix* matrix;
	Colour colour;

	Point left_normal(Point p0, Point p1);
	Point right_normal(Point p0, Point p1);

	std::vector<Facet> get_visible_facets();
	std::vector<Facet> draw_facets(SDL_Surface* s, std::vector<Facet> visible_facets);
public:
	Scene3D();
	Scene3D(const Scene3D& copy);
	~Scene3D();

	void add_object(Shape3D* object);
	Shape3D* get_object(int count);
	void clear_scene();
	int get_objects_count();
	void set_base_line(Line* line);
	void set_colour(Colour colour);

	void rotate_x(float d_angle);
	void rotate_y(float d_angle);
	void rotate_z(float d_angle);
	void rotate_vector(Vector vector, double d_angle);
	void move(float dx, float dy, float dz);
	void scale(float d_scale);

	void draw(SDL_Surface* s);

	Scene3D& operator=(const Scene3D& other);
};