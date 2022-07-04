#include "Graphic.h"
#include "Ini.h"
#include "Render.h"

float Graphic::Length(const Vector3f& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Vector3f Graphic::Normalize(const Vector3f& vec)
{
	Vector3f ret = vec;
	float l = Length(ret);
	if (ret.x != 0)
		ret.x /= l;
	if (ret.y != 0)
		ret.y /= l;
	if (ret.z != 0)
		ret.z /= l;
	return ret;
}

Vector3f Graphic::Rotate(const Vector3f& vec, const Vector3f& fi)
{
	Vector3f ret = vec;
	Vector3f buff = ret;
	//rotate on y axis
	ret.x = buff.x * cos(fi.y) + buff.z * sin(fi.y);
	ret.y = buff.y;
	ret.z = -buff.x * sin(fi.y) + buff.z * cos(fi.y);

	buff = ret;

	//rotate on z axis
	ret.x = cos(fi.z) * buff.x - buff.y * sin(fi.z);
	ret.y = sin(fi.z) * buff.x + buff.y * cos(fi.z);

	return ret;
}

Vector3f Graphic::Mix(const Vector3f& v1, const Vector3f& v2, const float& percent)
{
	Vector3f diff = Vector3f(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);

	return Vector3f(v1.x + diff.x * percent, v1.y + diff.y * percent, v1.z + diff.z * percent);;
}

ImageAccurate Graphic::RenderApproximate(ImageAccurate& dump, const Image& image, const Ini& setup, const Render& render)
{
	for (int i = 0; i < setup.h; i++)
		for (int j = 0; j < setup.w; j++)
		{
			Color c = image.getPixel(j, i);
			dump[i][j].x += (int(c.r) / (long double)render.render_samples) * render.samples_per_frame;
			dump[i][j].y += (int(c.g) / (long double)render.render_samples) * render.samples_per_frame;
			dump[i][j].z += (int(c.b) / (long double)render.render_samples) * render.samples_per_frame;
		}

	return dump;
}

Image Graphic::VectorToImage(const ImageAccurate& dump, const Ini& setup)
{
	Image img;
	img.create(setup.w, setup.h);
	for (int i = 0; i < setup.h; i++)
		for (int j = 0; j < setup.w; j++)
		{
			img.setPixel(j, i, Color(
				dump[i][j].x,
				dump[i][j].y,
				dump[i][j].z
			));
		}
	return img;
}

Vector3f Graphic::ValueToAngle(const Vector3f& vec)
{
	return Vector3f((vec.x / 3.14) * 180, (vec.y / 3.14) * 180, (vec.z / 3.14) * 180);
}

Vector3f Graphic::AngleToValue(const Vector3f& vec)
{
	return Vector3f((vec.x / 180) * 3.14, (vec.y / 180) * 3.14, (vec.z / 180) * 3.14);
}
