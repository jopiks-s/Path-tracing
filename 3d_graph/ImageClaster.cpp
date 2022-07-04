#include "ImageClaster.h"

ImageClaster::ImageClaster()
{
	claster = RectangleShape(Vector2f(0, 0));
}

ImageClaster::ImageClaster(RectangleShape claster) : claster(claster)
{}

ImageClaster& ImageClaster::Create(Vector2i location, Vector2i size)
{
	claster = RectangleShape(Vector2f(location.x, location.y));
	claster.setSize(Vector2f(size.x, size.y));
	claster.setFillColor(Color::Magenta);
	return *this;
}
