#include "WindowProp.h"

WindowProp::WindowProp(int w, int h)
{
	preFrame.create(w, h);
}

void WindowProp::calculate_fps(RenderWindow& window, string window_title)
{
	int fps = 1.0 / tick.getElapsedTime().asSeconds();
	window.setTitle(window_title + " | " + to_string(fps) + " FPS");
	tick.restart();
}
