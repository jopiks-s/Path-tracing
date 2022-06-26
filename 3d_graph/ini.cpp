#include "Ini.h"
#include "Render.h"

Ini::Ini(int w, int h, Vector3f light_dir, int sun_siz, int max_reflect,
	Render render, string render_parth)
	: w(w), h(h), light_dir(Graphic::Normalize(light_dir)), sun_size(sun_size), max_reflect(max_reflect),
	render(render), render_path(render_path)
{}
