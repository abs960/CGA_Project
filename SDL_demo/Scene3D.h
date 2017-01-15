#pragma once

#include "Shape3D.h"
#include "Line.h"
#include "Matrix.h"
#include "Colour.h"
#include "Scene.h"
#include "stdafx.h"

class Scene3D : public Scene {
protected:
	Point store_shift;
	Vector observer = Vector(0, 0, -1000);

	std::vector<Shape3D*> objects;
	Line* base_line;
	Matrix* matrix;
	Colour colour;
	bool lines_visible;

	Point left_normal(Point p0, Point p1);
	Point right_normal(Point p0, Point p1);

	virtual void apply_transformation();
	virtual std::vector<Facet> get_visible_facets();
	virtual std::vector<Facet> draw_facets(SDL_Surface* s, std::vector<Facet> visible_facets);
	void save_shift();
	void load_shift();
	void add_point_to_facet(Point p, Facet* f);
public:
	Scene3D();
	Scene3D(const Scene3D& copy);
	~Scene3D();

	void set_lines_visible(bool flag);
	void add_object(Shape3D* object);
	Shape3D* get_object(int count);
	void clear_scene();
	int get_objects_count();
	void set_base_line(Line* line);
	void set_colour(Colour colour);

	virtual void rotate_x(float d_angle);
	virtual void rotate_y(float d_angle);
	virtual void rotate_z(float d_angle);
	virtual void rotate_vector(Vector vector, float d_angle);
	virtual void move(float dx, float dy, float dz);
	virtual void scale(float d_scale);

	virtual void draw();

	Scene3D& operator=(const Scene3D& other);
};