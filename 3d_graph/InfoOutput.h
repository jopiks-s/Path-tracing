#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <time.h>
#include <cmath>

class Ini;
class Camera;
class Render;

using namespace sf;
using namespace std;

class InfoOutput
{
public:
	InfoOutput(string font_path, Color font_color, bool disable = false);

	bool disable;
	Font font;
	Color font_color;

	void Disable();
	void Enable();
	void Switch();
	bool draw(RenderWindow& window, const Ini& setup, const Camera& camera, const Render& render);
	bool draw_render(RenderWindow& window, const Ini& setup, int sample, const Clock& elapsed_time, const Render& render);
	bool draw_render_done(RenderWindow& window, const Ini& setup);
};