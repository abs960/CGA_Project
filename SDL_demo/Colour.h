#pragma once

#include "SDL.h"
#include "stdlib.h"
#include "time.h"

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(r) << 8) | g) << 8) | b)

class Colour {
private:
	double intensity;
	Uint32 value;
public:
	static const Uint32 COLOUR_BLACK = 0x00000000;
	static const Uint32 COLOUR_WHITE = 0x00FFFFFF;
	static const Uint32 COLOUR_RED = 0x00FF0000;
	static const Uint32 COLOUR_GREEN = 0x0000FF00;
	static const Uint32 COLOUR_BLUE = 0x000000FF;
	static const Uint32 COLOUR_ORANGE = 0x00FF9933;
	static const Uint32 COLOUR_PINK = 0x00FF00FF;

	Colour();
	Colour(unsigned int red, unsigned int green, unsigned int blue);
	Colour(Uint32 value);
	Colour(const Colour& copy);
	~Colour();

	Uint32 get_value();
	void set_value(Uint32 value);
	unsigned int get_red();
	unsigned int get_green();
	unsigned int get_blue();
	void change_intensity(double new_intensity);

	Colour& operator=(const Colour& other);

	static Colour get_random_colour();
};

