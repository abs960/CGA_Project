#pragma once

#include "SDL.h"
#include "stdafx.h"
#include <vector>
#include "Controller2D.h"

class Window {
public:
	Window();
	Window(unsigned int w, unsigned int h);
	Window(const Window& copy);
	~Window();

	Window& operator=(const Window& other);

	virtual void set_width(unsigned int w);
	virtual void set_height(unsigned int h);
	virtual unsigned int get_width();
	virtual unsigned int get_height();
	virtual bool init();
	virtual bool run();
	virtual void close();
protected:
	static const unsigned int DEFAULT_WIDTH = 512;
	static const unsigned int DEFAULT_HEIGHT = 512;
	SDL_Window* gWindow;
	unsigned int width;
	unsigned int height;
	std::vector<Controller*> controllers;
	int active_controller_number;

	virtual void add_controller(Controller* controller);
};

