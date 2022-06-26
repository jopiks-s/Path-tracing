#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<long double>>>;

class Ini;

namespace Render
{
	bool save_result(const ImageAccurate& render_dump, const Clock render_elapsed_time, const Ini& setup);
}

class WindowProp
{
	int frame = 0, render_frame = 0, fixed_frame_counter = 1;
	int current_samples = 0;
	Clock elapsed_time, render_elapsed_time;

	bool render = false,
		focus = false;

	Texture preFrame;
	int fsdfsd;
};