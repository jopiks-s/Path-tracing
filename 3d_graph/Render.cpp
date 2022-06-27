#include "Render.h"
#include "Ini.h"
#include "sfml_extension.h"
#include "WindowProp.h"
#include "Camera.h"

Render::Render(int viewport_samples, int render_samples, int MAX_CLASTER)
	: viewport_samples(viewport_samples), render_samples(render_samples), MAX_CLASTER(MAX_CLASTER)
{
	if (MAX_CLASTER % 2 == 1)
		this->MAX_CLASTER++;
}

void Render::choose_claster_size(const WindowProp& window_prop)
{
	if (rendering)
	{
		bool correct = false;
		claster_size = MAX_CLASTER;
		do
		{
			if (render_samples - window_prop.render_frame < claster_size)
				claster_size /= 2;
			else
				correct = true;
		} while (!correct);
	}
	else if (window_prop.focus)
		if (window_prop.fixed_frame_counter < 10)
			claster_size = 1;
		else
			claster_size = viewport_samples;
	else
		claster_size = 1;
}
