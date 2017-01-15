#include "Scene.h"

void Scene::set_surface(SDL_Surface * s) {
	surface = s;
}

SDL_Surface * Scene::get_surface() {
	return surface;
}

void Scene::set_background_colour(Colour * c) {
	background_colour = c;
}

Colour * Scene::get_background_colour() {
	return background_colour;
}

Scene::~Scene() {
	delete background_colour;
	delete matrix;
}
