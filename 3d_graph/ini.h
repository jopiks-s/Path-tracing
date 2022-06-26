#pragma once

#include <SFML/Graphics.hpp>
#include "Graphic.h"

using namespace sf;
using namespace std;

class Render;

class Ini
{
public:
	int w, h;

	Vector3f light_dir;
	int sun_size;

	int max_reflect;
	Render render;

	string render_path;

	Ini(int w, int h, Vector3f light_dir, int sun_siz, int max_reflect,
		Render render, string render_parth);
};