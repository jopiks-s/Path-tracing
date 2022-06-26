#include "WindowProp.h"

WindowProp::WindowProp(int w, int h)
{
	preFrame.create(w, h);
}


void WindowProp::choose_samples(int viewport_samples)
{
	if (render)
		current_samples = 1;
	else if (focus)
		if (fixed_frame_counter < 10)
			current_samples = 1;
		else
			current_samples = viewport_samples;
	else
		current_samples = 1;
}