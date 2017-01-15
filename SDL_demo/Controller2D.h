#pragma once

#include "Controller.h"
#include "Scene2D.h"

class Controller2D : public Controller {
public:
	Controller2D();

	virtual bool init();
	virtual void handle_event(SDL_Event e);

	~Controller2D();
protected:
	Scene2D * scene;

	virtual void init_scene(SDL_Surface* surface);
};

