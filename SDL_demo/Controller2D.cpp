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
	
}

void Controller2D::draw() {
	scene->draw();
}

Controller2D::~Controller2D() {
	delete scene;
}

bool Controller2D::init_scene() {
	scene = new Scene2D();

	return true;
}
