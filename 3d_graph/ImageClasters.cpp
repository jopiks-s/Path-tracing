#include "ImageClasters.h"
#include "Render.h"
#include "Ini.h"

ImageClasters::ImageClasters()
{
	claster = RectangleShape(Vector2f(0, 0));
	claster.setFillColor(Color::Magenta);
}

void ImageClasters::create_clasters(const Render& render, const Ini& setup)
{
	int max_claster = render.MAX_CLASTER_SIZE;
	int width_counter = ceil(float(setup.w) / max_claster);
	int height_counter = ceil(float(setup.h) / max_claster);

	clasters_info = vector<vector<Vector2i>>(width_counter * height_counter, vector<Vector2i>(2, Vector2i()));

	for (int y = 0; y < height_counter; y++)
		for (int x = 0; x < width_counter; x++)
		{
			Vector2i block_position = Vector2i(x * max_claster, y * max_claster);
			Vector2i block_size = Vector2i
			(
				x + 1 != width_counter ? max_claster : setup.w - (width_counter - 1) * max_claster,
				y + 1 != height_counter ? max_claster : setup.h - (height_counter - 1) * max_claster
			);
			clasters_info[y * height_counter + x][0] = block_position;
			clasters_info[y * height_counter + x][1] = block_size;
		}
	SelectClaster(0);
}

void ImageClasters::SelectClaster(int index = 0)
{
	if (index >= clasters_info.size())
		throw "Index out of range";

	auto pos = clasters_info[index][0];
	auto size = clasters_info[index][1];
	claster.setPosition(pos.x, pos.y);
	claster.setSize(Vector2f(size.x, size.y));

	claster_pointer = index;
	samples_counter = 0;
}

void ImageClasters::NextClaster()
{
	SelectClaster(claster_pointer + 1);
}

bool ImageClasters::IsLast()
{
	if (claster_pointer == clasters_info.size() - 1)
		return true;
	return false;
}
