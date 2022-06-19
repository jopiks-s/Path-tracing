#pragma once

#include <SFML/Graphics.hpp>
#include "Graphic.h"
#include <map>

using namespace sf;
using namespace std;

class Ini
{
public:
	int w, h;

	Vector3f light_dir;
	int sun_size;

	int max_reflect;
	int render_samples;
	int viewport_samples;

	string render_path;

	bool disable = false;
	map<string, Text> info_text;

	Ini(int w, int h, Vector3f light_dir, int sun_siz, int max_reflect,
		int render_samples, int viewport_samples, string render_parth);

	bool Draw(RenderWindow& window);
};