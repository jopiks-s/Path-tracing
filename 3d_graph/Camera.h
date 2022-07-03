#pragma once
#include <SFML/Graphics.hpp>
#include "Graphic.h"
#include "Ini.h"

using namespace sf;

class Camera
{
public:

	double sensitivity,
		camera_speed,
		focal_length,
		aperture,
		camera_size;
	Vector3f camera_origin,
		camera_rotation,
		fly_dir;

	bool disable;

	//receive camera_rotation in degree
	Camera(double sensitivity, double camera_speed, double focal_length, double aperture, double camera_size, Vector3f camera_origin, Vector3f camera_rotation, bool disable = false);

	bool RotateCamera(const Event& e, const RenderWindow& window, const Ini& setup);
	bool MoveCamera();

	bool KeyboardInputRecord(const Event& e);

	void Disable();
	void Enable();
};