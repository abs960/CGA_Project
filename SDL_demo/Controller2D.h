#pragma once

#include "Controller.h"
#include "Scene2D.h"

class Controller2D : public Controller {
public:
	Controller2D();
	Controller2D(const Controller2D& copy);
	Controller2D& operator=(const Controller2D& other);

	virtual bool init(SDL_Window* gWindow, int width, int height);
	virtual void handle_event(SDL_Event e);
	virtual void draw();

	~Controller2D();
protected:
	Scene2D * scene;

	virtual bool init_scene();
};

