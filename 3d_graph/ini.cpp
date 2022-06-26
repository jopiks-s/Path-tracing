#include "Ini.h"
#include "Render.h"

Ini::Ini(int w, int h, Vector3f light_dir, int sun_size, int max_reflect, string render_path)
	: w(w), h(h), light_dir(Graphic::Normalize(light_dir)), sun_size(sun_size),
	max_reflect(max_reflect), render_path(render_path)
{}
