#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<long double>>>;

class Ini;
class WindowProp;
class Camera;
class Render;

class Render
{
public:
	int claster_size = 0,
		sun_size,
		max_reflect,
		max_reflect_buffer = 0,
		viewport_samples,
		render_samples;
	const int MAX_CLASTER;
	bool rendering = false,
		simplified = false;

	Render(int viewport_samples, int render_samples, int MAX_CLASTER, int sun_size, int max_reflect);
	void switch_image_quality();
	void choose_claster_size(const WindowProp& window_prop);

	void StartRender();

	void set_uniforms(Shader& shader, const WindowProp& window_prop, const Ini& setup, const Camera& camera, const Texture& lol);
	bool save_result(const ImageAccurate& render_dump, const Clock& render_elapsed_time, const Ini& setup);
};