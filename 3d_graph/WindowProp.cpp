#include "WindowProp.h"
#include "Ini.h"

WindowProp::WindowProp(int w, int h)
{
	preFrame.create(w, h);
	if (w != 1920 || h != 1080)
		resizable = false;
}

void WindowProp::calculate_fps(RenderWindow& window, string window_title)
{
	int fps = 1.0 / tick.getElapsedTime().asSeconds();
	window.setTitle(window_title + " | " + to_string(fps) + " FPS");
	tick.restart();
}

void WindowProp::enable_fullscrean(RenderWindow& window, const Ini& setup)
{
	if (fullscrean || !resizable)
	{
		cout << "Can't enable fullscrean\n";
		return;
	}

	window.create(VideoMode(setup.w, setup.h), "bebe", Style::Fullscreen);
	fullscrean = true;
	window.setMouseCursorVisible(false);
}

void WindowProp::disable_fullscrean(RenderWindow& window, const Ini& setup)
{
	if (!fullscrean)
		return;

	window.create(VideoMode(setup.w, setup.h), "bebe", resizable ? Style::Default : Style::Titlebar | Style::Close);
	fullscrean = false;
	window.setMouseCursorVisible(true);
}
