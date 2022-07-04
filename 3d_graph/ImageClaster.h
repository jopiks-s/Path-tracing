#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class ImageClaster
{
public:
	RectangleShape claster;
	int samples_counter = 0;

	ImageClaster();
	ImageClaster(RectangleShape claster);

	ImageClaster& Create(Vector2i location, Vector2i size);
};