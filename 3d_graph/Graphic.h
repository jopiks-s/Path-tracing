#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Ini;

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<double>>>;

namespace Graphic
{
	float Length(const Vector3f& vec);
	Vector3f Normalize(const Vector3f& vec);
	Vector3f Rotate(const Vector3f& vec, const Vector3f& fi);
	Vector3f Mix(const Vector3f& v1, const Vector3f& v2, const float& percent);

	ImageAccurate RenderApproximate(ImageAccurate& dump, const Image& image, const Ini& setup);
	Image VectorToImage(const ImageAccurate& dump, const Ini& setup);

	void Print(const Vector3f& vec);
}
