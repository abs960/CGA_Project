#include "Controller.h"

Controller::Controller() {
	gRenderer = NULL;
	gTexture = NULL;
	surface = NULL;
}

Controller::~Controller() {
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	surface = NULL;
}

bool Controller::init(SDL_Window* gWindow, int width, int height) {
	return init_SDL(gWindow, width, height);
}

bool Controller::init_SDL(SDL_Window* gWindow, int width, int height) {
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (NULL == gTexture) {
		printf("Failed to load media!\n");
		return false;
	}

	return true;
}