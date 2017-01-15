#include "Controller2D.h"

Controller2D::Controller2D() {}

bool Controller2D::init() {
	if (init_successful) {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		gWindow = SDL_CreateWindow("CGA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								   SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
		if (gRenderer == NULL) {
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}
	}
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Surface* surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000);
	gTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (NULL == gTexture) {
		printf("Failed to load media!\n");
		return false;
	}

	init_scene(surface);
}

void Controller2D::handle_event(SDL_Event e) {
	
}

Controller2D::~Controller2D() {
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	delete scene;
	SDL_Quit();
}

void Controller2D::init_scene(SDL_Surface* surface) {

}
