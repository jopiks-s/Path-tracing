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