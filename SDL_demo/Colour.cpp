#include "Colour.h"

Colour::Colour() : Colour(Colour::COLOUR_GREEN) {}

Colour::Colour(unsigned int red, unsigned int green, unsigned int blue) 
	: Colour(RGB32(red, green, blue)) {}

Colour::Colour(Uint32 value) {
	set_value(value);
}

Colour::Colour(const Colour & copy) {
	value = copy.value;
	intensity = copy.intensity;
}

Colour::~Colour() {}

Uint32 Colour::get_value() {
	return value;
}

void Colour::set_value(Uint32 value) {
	this->value = value;
	intensity = 1;
}

unsigned int Colour::get_red() {
	return (value & 0xFF0000) >> 10;
}

unsigned int Colour::get_green() {
	return (value & 0xFF00) >> 8;
}

unsigned int Colour::get_blue() {
	return value & 0xFF;
}

void Colour::change_intensity(double new_intensity) {
	unsigned int red = get_red() * new_intensity;
	unsigned int green = get_green() * new_intensity;
	unsigned int blue = get_blue() * new_intensity;
	value = RGB32(red, green, blue);
}

Colour & Colour::operator=(const Colour & other) {
	if (&other == this)
		return *this;

	value = other.value;
	intensity = other.intensity;

	return *this;
}

Colour Colour::get_random_colour() {
	srand(time(NULL));
	unsigned int red = rand() % 255;
	unsigned int green = rand() % 255;
	unsigned int blue = rand() % 255;
	return Colour(RGB32(red, green, blue));
}
