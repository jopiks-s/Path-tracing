#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<double>>>;

namespace Graphic
{
	float Length(const Vector3f& vec);
	Vector3f Normalize(const Vector3f& vec);
	Vector3f Rotate(const Vector3f& vec, const Vector3f& fi);
	Vector3f Mix(const Vector3f& v1, const Vector3f& v2, const float& percent);

	ImageAccurate RenderApproximate(ImageAccurate& dump, const Image& image, int samples, int w, int h);
	Image VectorToImage(const ImageAccurate& dump, int w, int h);

	void Print(const Vector3f& vec);
}
