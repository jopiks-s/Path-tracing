#pragma once
#include <SFML/Graphics.hpp>
#include "Graphic.h"
#include "Ini.h"

using namespace sf;

class Camera
{
public:

	double sensitivity;
	double camera_speed;
	double focal_distance;
	double camera_size;
	Vector3f camera_origin;
	Vector3f camera_rotation;
	Vector3f fly_dir;

	bool disable;

	Camera(double sensitivity, double camera_speed, double focal_distance, double camera_size, Vector3f camera_origin, Vector3f camera_rotation, bool disable = false);

	bool RotateCamera(const Event& e, const RenderWindow& window, const Ini& setup);
	bool MoveCamera();

	void KeyboardInputRecord(const Event& e);

	void Disable();
	void Enable();
};