#include "WindowProp.h"
#include "Ini.h"

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

void WindowProp::enable_fullscrean(RenderWindow& window, const Ini& setup)
{
	if (fullscrean)
		return;

	window.create(VideoMode(setup.w, setup.h), "bebe", Style::Fullscreen);
	fullscrean = true;
	window.setMouseCursorVisible(false);
}

void WindowProp::disable_fullscrean(RenderWindow& window, const Ini& setup)
{
	if (!fullscrean)
		return;

	window.create(VideoMode(setup.w, setup.h), "bebe", Style::Default);
	fullscrean = false;
	window.setMouseCursorVisible(true);
}
