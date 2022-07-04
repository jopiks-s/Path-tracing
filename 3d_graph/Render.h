#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include "ImageClaster.h"

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<long double>>>;

class Ini;
class WindowProp;
class Camera;
class Render;
class ImageClaster;

class Render
{
public:
	int samples_per_frame = 0,
		sun_size,
		max_reflect,
		max_reflect_buffer = 0,
		viewport_samples,
		render_samples;
	vector<vector<ImageClaster>> clasters;
	ImageClaster* claster_pointer;
	const int MAX_SAMPLES_PER_FRAME,
		MAX_CLASTER_SIZE;
	bool rendering = false,
		simplified = false;

	Render(int viewport_samples, int render_samples, int MAX_SAMPLES_PER_FRAME, int MAX_CLASTER_SIZE, int sun_size, int max_reflect);
	void switch_image_quality();
	void choose_samples_amount(const WindowProp& window_prop);

	void StartRender(const Ini& setup);// , const WindowProp& window_prop);
	void render_claster();

	void set_uniforms(Shader& shader, const WindowProp& window_prop, const Ini& setup, const Camera& camera, const Texture& lol);
	bool save_result(const ImageAccurate& render_dump, const Clock& render_elapsed_time, const Ini& setup);
};