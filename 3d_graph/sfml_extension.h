#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

namespace sf
{
	void Print(const Vector3f& vec);
	void draw_img(RenderWindow& window, const Image& img);

	vector<int> FormatTime(const Time& t);
	string TimeToString(const vector<int>& v, char delimiter = ':', bool add_unit = false);

	Vector2i v2f_to_v2i(const Vector2f& vec);
}