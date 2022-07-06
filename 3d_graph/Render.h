#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include "ImageClasters.h"

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<long double>>>;

class Ini;
class WindowProp;
class Camera;
class Render;
class ImageClasters;

class Render
{
public:
	int	sun_size,
		max_reflect,
		max_reflect_buffer = 0,

		samples_per_frame = 0,
		viewport_samples,
		render_samples;
	const int MAX_SAMPLES_PER_FRAME,
		MAX_CLASTER_SIZE;

	ImageClasters image_clasters;

	bool rendering = false,
		simplified = false;

	Render(int viewport_samples, int render_samples, int MAX_SAMPLES_PER_FRAME, int MAX_CLASTER_SIZE, int sun_size, int max_reflect);
	void switch_image_quality();
	void choose_samples_amount(const WindowProp& window_prop);

	void StartRender(const Ini& setup, WindowProp& window_prop);
	void EndRender();
	bool render_claster(RenderWindow& window, const Shader& shader, WindowProp& window_prop, const Ini& setup);
	void update_preFrame(WindowProp& window_prop);

	void set_uniforms(Shader& shader, const WindowProp& window_prop, const Ini& setup, const Camera& camera, const Texture& lol);
	bool save_result(const ImageAccurate& render_dump, const Clock& render_elapsed_time, const Ini& setup);
};