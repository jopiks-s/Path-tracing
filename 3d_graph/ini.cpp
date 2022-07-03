#include "Ini.h"
#include "Render.h"

Ini::Ini(int w, int h, Vector3f light_dir, string render_path)
	: w(w), h(h), light_dir(Graphic::Normalize(light_dir)), render_path(render_path)
{}
