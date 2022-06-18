#pragma once
#include <SFML/Graphics.hpp>
#include "Graphic.h"

using namespace sf;

extern int w, h;

class Camera
{
public:

	double sensitivity;
	double camera_speed;
	Vector3f camera_origin;
	Vector3f camera_rotation;
	Vector3f fly_dir;

	bool disable;

	Camera(double sensitivity, double camera_speed, Vector3f camera_origin, Vector3f camera_rotation, bool disable = false)
		: sensitivity(sensitivity), camera_speed(camera_speed), camera_origin(camera_origin), camera_rotation(camera_rotation),
		fly_dir(0,0,0), disable(disable)
	{}

	bool RotateCamera(const Event& e, const RenderWindow& window);
	bool MoveCamera();

	void KeyboardInputRecord(const Event& e);

	void Disable();
	void Enable();
};