#include "Camera.h"

Camera::Camera(double sensitivity, double camera_speed, double focal_length, double aperture, double camera_size, Vector3f camera_origin, Vector3f camera_rotation, bool disable)
	: sensitivity(sensitivity), camera_speed(camera_speed), focal_length(focal_length), aperture(aperture), camera_size(camera_size), camera_origin(camera_origin),
	camera_rotation(Graphic::AngleToValue(camera_rotation)), fly_dir(0, 0, 0), disable(disable)
{}

bool Camera::RotateCamera(const Event& e, const RenderWindow& window, const Ini& setup)
{
	if (disable)
		return false;

	float difx = ((e.mouseMove.x - setup.w / 2) / (float)setup.w) * sensitivity;
	float dify = ((e.mouseMove.y - setup.h / 2) / (float)setup.h) * sensitivity;

	if (difx == 0 && dify == 0)
		return false;

	camera_rotation.z += difx;
	camera_rotation.y += dify;
	Mouse::setPosition(Vector2i(setup.w / 2, setup.h / 2), window);

	return true;
}

bool Camera::MoveCamera()
{
	if (disable)
		return false;
	if (Graphic::Length(fly_dir) == 0)
		return false;

	camera_origin.z += fly_dir.z * camera_speed;

	int z_buff = fly_dir.z;
	fly_dir.z = 0;

	Vector3f buff_v = Graphic::Normalize(fly_dir);
	buff_v = Graphic::Rotate(buff_v, camera_rotation);
	camera_origin.x += buff_v.x * camera_speed;
	camera_origin.y += buff_v.y * camera_speed;
	camera_origin.z += buff_v.z * camera_speed;

	fly_dir.z = z_buff;

	return true;
}

bool Camera::KeyboardInputRecord(const Event& e)
{
	if (disable)
		return false;
	bool change = false;
	if (e.type == Event::KeyPressed)
	{
		if (e.key.code == Keyboard::W || e.key.code == Keyboard::S || e.key.code == Keyboard::A
			|| e.key.code == Keyboard::D || e.key.code == Keyboard::Space
			|| e.key.code == Keyboard::LShift || e.key.code == Keyboard::Num1 || e.key.code == Keyboard::Num2)
			change = true;
		if (e.key.code == Keyboard::W)
			fly_dir.x = 1;
		if (e.key.code == Keyboard::S)
			fly_dir.x = -1;
		if (e.key.code == Keyboard::A)
			fly_dir.y = -1;
		if (e.key.code == Keyboard::D)
			fly_dir.y = 1;
		if (e.key.code == Keyboard::Space)
			fly_dir.z = 1;
		if (e.key.code == Keyboard::LShift)
			fly_dir.z = -1;
		if (e.key.code == Keyboard::Num1)
			aperture += 5;
		if (e.key.code == Keyboard::Num2)
			aperture -= 5;
	}

	if (e.type == Event::KeyReleased)
	{
		if (e.key.code == Keyboard::W || e.key.code == Keyboard::S || e.key.code == Keyboard::A
			|| e.key.code == Keyboard::D || e.key.code == Keyboard::Space || e.key.code == Keyboard::LShift)
			change = true;
		if (e.key.code == Keyboard::W)
			fly_dir.x = 0;
		if (e.key.code == Keyboard::S)
			fly_dir.x = 0;
		if (e.key.code == Keyboard::A)
			fly_dir.y = 0;
		if (e.key.code == Keyboard::D)
			fly_dir.y = 0;
		if (e.key.code == Keyboard::Space)
			fly_dir.z = 0;
		if (e.key.code == Keyboard::LShift)
			fly_dir.z = 0;
	}

	if (e.type == Event::MouseWheelMoved)
	{
		focal_length += e.mouseWheel.delta * 0.1;
		change = true;
	}

	return change;
}

void Camera::Disable()
{
	disable = true;
	fly_dir = Vector3f(0, 0, 0);
}

void Camera::Enable()
{
	disable = false;
}
