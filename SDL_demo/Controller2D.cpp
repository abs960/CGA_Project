#include "Controller2D.h"

Controller2D::Controller2D() {}

Controller2D::Controller2D(const Controller2D & copy) {
	gRenderer = copy.gRenderer;
	gTexture = copy.gTexture;
	surface = copy.surface;
	*scene = *(copy.scene);
}

Controller2D & Controller2D::operator=(const Controller2D & other) {
	if (this == &other)
		return *this;

	gRenderer = other.gRenderer;
	gTexture = other.gTexture;
	surface = other.surface;
	*scene = *(other.scene);

	return *this;
}

bool Controller2D::init(SDL_Window * gWindow, int width, int height) {
	return init_SDL(gWindow, width, height) && init_scene();
}

void Controller2D::handle_event(SDL_Event e) {
	switch (e.key.keysym.scancode) {
		// Movement
	case SDL_SCANCODE_UP:
		if (separate_control) {
			MatrixShape2D* s = scene->get_object(active_shape_count);
			s->do_move(0, -STEP_T);
		} else
			scene->move(0, -STEP_T, 0);
		break;
	case SDL_SCANCODE_DOWN:
		if (separate_control) {
			MatrixShape2D* s = scene->get_object(active_shape_count);
			s->do_move(0, STEP_T);
		} else
			scene->move(0, STEP_T, 0);
		break;
	case SDL_SCANCODE_LEFT:
		if (separate_control) {
			MatrixShape2D* s = scene->get_object(active_shape_count);
			s->do_move(-STEP_T, 0);
		} else
			scene->move(-STEP_T, 0, 0);
		break;
	case SDL_SCANCODE_RIGHT:
		if (separate_control) {
			MatrixShape2D* s = scene->get_object(active_shape_count);
			s->do_move(STEP_T, 0);
		} else
			scene->move(STEP_T, 0, 0);
		break;
		// Rotation
	case SDL_SCANCODE_Q:
		if (separate_control) {
			MatrixShape2D* s = scene->get_object(active_shape_count);
			s->do_rotate(-STEP_R);
		} else
			scene->rotate(-STEP_R);
		break;
	case SDL_SCANCODE_E:
		if (separate_control) {
			MatrixShape2D* s = scene->get_object(active_shape_count);
			s->do_rotate(STEP_R);
		} else
			scene->rotate(STEP_R);
		break;
		// Scaling
	case SDL_SCANCODE_SLASH:
		if (separate_control) {
			MatrixShape2D* s = scene->get_object(active_shape_count);
			s->do_scale(STEP_S);
		} else
			scene->scale(STEP_S);
		break;
	case SDL_SCANCODE_PERIOD:
		if (separate_control) {
			MatrixShape2D* s = scene->get_object(active_shape_count);
			s->do_scale(-STEP_S);
		} else
			scene->scale(-STEP_S);
		break;
		// New objects addition
	case SDL_SCANCODE_N: {
		NestedShape2D* n = new NestedShape2D();
		n->set_colour(Colour(Colour::COLOUR_GREEN));
		n->set_brush(new WuLine());
		scene->add_object(n);
		break;
	}
		// TODO: implement the addition of the rest of the shape types

		// Separate control of each shape OR Control all scene at once
	case SDL_SCANCODE_COMMA:
		separate_control = !separate_control;
		break;
	case SDL_SCANCODE_EQUALS:
		if (separate_control && active_shape_count < scene->get_objects_count() - 1) 
			active_shape_count++;
		break;
	case SDL_SCANCODE_MINUS:
		if (separate_control && active_shape_count > 0)
			active_shape_count--;
		break;
	}
}

void Controller2D::draw() {
	SDL_RenderClear(gRenderer);
	scene->draw(); 
	SDL_UpdateTexture(gTexture, NULL, surface->pixels, surface->pitch);
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
}

Controller2D::~Controller2D() {
	delete scene;
}

bool Controller2D::init_scene() {
	scene = new Scene2D();
	scene->set_surface(surface);
	scene->set_background_colour(new Colour(Colour::COLOUR_BLACK));
	NestedShape2D* n = new NestedShape2D(3, 150);
	n->set_colour(Colour(Colour::COLOUR_GREEN));
	n->set_brush(new WuLine());
	scene->add_object(n);
	active_shape_count = 0;

	return true;
}
