#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <time.h>
#include <cmath>

class Ini;

using namespace sf;
using namespace std;

class InfoOutput
{
public:
	InfoOutput(string font_path, bool disable = false);

	bool disable;

	Font font;
	void Disable();
	void Enable();
	void Switch();
	bool draw(RenderWindow& window, const Ini& setup);
	bool render_draw(RenderWindow& window, const Ini& setup, int sample, const Clock& elapsed_time);
};