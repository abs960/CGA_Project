#pragma once

#include "Scene.h"
#include "MatrixShape2D.h"

class Scene2D : public Scene{
public:
	Scene2D();
	Scene2D(const Scene2D& copy);
	~Scene2D();

	virtual void add_object(MatrixShape2D* s);
	virtual MatrixShape2D* get_object(int pos);
	virtual void remove_object(int pos);
	virtual void clear_scene();
	virtual int get_objects_count();

	void rotate(float d_angle);
	virtual void rotate_x(float d_angle);
	virtual void rotate_y(float d_angle);
	virtual void rotate_z(float d_angle);
	virtual void rotate_vector(Vector v, float d_angle);
	virtual void move(float dx, float dy, float dz);
	virtual void scale(float d_scale);

	virtual void draw();

	Scene2D& operator=(const Scene2D& other);
protected:
	std::vector<MatrixShape2D*> objects;

	void clear_objects();
	void clear_matrix();
	void apply_matrix(Matrix m);
};

