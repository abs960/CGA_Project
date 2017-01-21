#include "Window.h"

Window::Window() : Window(DEFAULT_WIDTH, DEFAULT_HEIGHT) {}

Window::Window(unsigned int w, unsigned int h) {
	gWindow = NULL;
	width = w;
	height = h;
}

Window::Window(const Window & copy) {
	gWindow = copy.gWindow;
	width = copy.width;
	height = copy.height;
}

Window::~Window() {}

Window & Window::operator=(const Window & other) {
	if (this == &other)
		return *this;

	gWindow = other.gWindow;
	width = other.width;
	height = other.height;

	return *this;
}

void Window::set_width(unsigned int w) {
	width = w;
}

void Window::set_height(unsigned int h) {
	height = h;
}

unsigned int Window::get_width() {
	return width;
}

unsigned int Window::get_height() {
	return height;
}

bool Window::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	gWindow = SDL_CreateWindow("CGA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								width, height, SDL_WINDOW_ALLOW_HIGHDPI);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	add_controller(new Controller2D());

	return true;
}

bool Window::run() {
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (SDL_QUIT == e.type) {
				quit = true;
			}
			if (SDL_KEYDOWN == e.type) {
				switch (e.key.keysym.scancode) {
				default:
					break;
				}
				controllers.at(active_controller_number)->handle_event(e);
				controllers.at(active_controller_number)->draw();
			}
		}
	}
	return true;
}

void Window::close() {
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

void Window::add_controller(Controller * controller) {
	controller->init(gWindow, width, height);
	controller->draw();
	controllers.push_back(controller);
	active_controller_number = controllers.size() - 1;
}
