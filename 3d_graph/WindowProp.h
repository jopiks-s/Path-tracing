#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class WindowProp
{
public:
	int frame = 0, render_frame = 0, fixed_frame_counter = 1;
	Clock tick, render_elapsed_time;

	bool focus = false;

	Texture preFrame;

	WindowProp(int w, int h);
	void calculate_fps(RenderWindow& window, string window_title);
};