#pragma once

#include "SDL.h"
#include "Line.h"
#include "Point.h"
#include "Colour.h"
#include "Matrix.h"
#include <vector>

class Scene {
public:
	void set_surface(SDL_Surface* s);
	SDL_Surface* get_surface();
	void set_background_colour(Colour* c);
	Colour* get_background_colour();

	virtual void rotate_x(float d_angle) = 0;
	virtual void rotate_y(float d_angle) = 0;
	virtual void rotate_z(float d_angle) = 0;
	virtual void rotate_vector(Vector v, float d_angle) = 0;
	virtual void move(float dx, float dy, float dz) = 0;
	virtual void scale(float d_scale) = 0;

	virtual void draw() = 0;

	~Scene();
protected:
	SDL_Surface * surface;
	Colour * background_colour;
	Matrix * matrix;
};
