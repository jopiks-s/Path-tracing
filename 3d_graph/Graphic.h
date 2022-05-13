#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

namespace Graphic
{
	float Length(const Vector3f& vec);
	Vector3f Normalize(const Vector3f& vec);
	Vector3f Rotate(const Vector3f& vec, const Vector3f& fi);
	Vector3f Mix(const Vector3f& v1, const Vector3f& v2, const float& percent);
	void Print(const Vector3f&vec);
}
