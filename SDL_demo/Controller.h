#pragma once

#include "SDL.h"

class Controller {
public:
	Controller();

	virtual bool init() = 0;
	virtual void handle_event(SDL_Event e) = 0;
protected:
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	SDL_Texture* gTexture;

	bool init_successful;
};

