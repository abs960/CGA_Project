#pragma once

#include "SDL.h"
#include "stdafx.h"
#include "Line.h"
#include "BrezenheimLine.h"
#include "WuLine.h"
#include "CyrusBeckLine.h"

class Controller {
public:
	Controller();
	~Controller();

	virtual bool init(SDL_Window* gWindow, int width, int height);
	virtual void handle_event(SDL_Event e) = 0;
	virtual void draw() = 0;
protected:
	SDL_Renderer* gRenderer;
	SDL_Texture* gTexture;
	SDL_Surface* surface;

	virtual bool init_SDL(SDL_Window* gWindow, int width, int height);
};

