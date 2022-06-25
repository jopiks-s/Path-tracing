#include "sfml_extension.h"

void sf::Print(const Vector3f& vec)
{
	std::cout << "x: " + std::to_string(vec.x) + " y: " + std::to_string(vec.y) + " z: " + std::to_string(vec.z) + "\n";
}

void sf::draw_img(RenderWindow& window, const Image& img)
{
	Texture t;
	t.loadFromImage(img);
	Sprite s;
	s.setTexture(t);
	window.draw(s);
}


vector<int> sf::FormatTime(const Time& t)
{
	int sec = floor(t.asSeconds());
	int mili = floor(100 * (t.asSeconds() - double(sec)));
	int min = floor(double(sec) / 60.0);
	sec -= min * 60;

	return vector<int>({ min, sec, mili });
}

string sf::TimeToString(const vector<int>& v, char delimiter, bool add_unit)
{
	string output;
	if (add_unit)
		output = to_string(v[0]) + "m" + delimiter + to_string(v[1]) + "s" + delimiter + to_string(v[2]) + "ms";
	else
		output = to_string(v[0]) + delimiter + to_string(v[1]) + delimiter + to_string(v[2]);

	return output;
}