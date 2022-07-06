#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

class Ini;
class Render;

class ImageClasters
{
public:
	RectangleShape claster;
	int samples_counter = 0,
		claster_pointer = 0;
	vector<vector<Vector2i>> clasters_info;

	ImageClasters();

	void create_clasters(const Render& render, const Ini& setup);
	void SelectClaster(int index);
	void NextClaster();
	bool IsLast();
};