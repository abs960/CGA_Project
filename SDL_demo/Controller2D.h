#pragma once

#include "Controller.h"
#include "Scene2D.h"
#include "MatrixShape2D.h"
#include "SectionWindow.h"
#include "NestedShape2D.h"

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
	// TODO: use flags and bit operations for flags, remove this 
	bool separate_control = false;

	int active_shape_count = -1;

	const float STEP_T = 5;
	const float STEP_R = 5;
	const float STEP_S = 0.05;
	Scene2D * scene;

	virtual bool init_scene();
};

