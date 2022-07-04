#include "Render.h"
#include "Ini.h"
#include "sfml_extension.h"
#include "WindowProp.h"
#include "Camera.h"

Render::Render(int viewport_samples, int render_samples, int MAX_SAMPLES_PER_FRAME, int MAX_CLASTER_SIZE, int sun_size, int max_reflect)
	: viewport_samples(viewport_samples), render_samples(render_samples),
	MAX_SAMPLES_PER_FRAME(MAX_SAMPLES_PER_FRAME > 256 ? 256 : MAX_SAMPLES_PER_FRAME),
	MAX_CLASTER_SIZE(MAX_CLASTER_SIZE),
	sun_size(sun_size), max_reflect(max_reflect)
{
	if (MAX_SAMPLES_PER_FRAME > 256)
		cout << "GPU CAN'T HANDLE THIS SAMPLES PER FRAME: " << MAX_SAMPLES_PER_FRAME << '\n'
		<< "SAMPLES_PER_FRAME SET TO 256 \n"
		<< "To change max samples per frame on gpu go to 'Shader.frag', line: 10, and change array size \n";
}

void Render::switch_image_quality()
{
	if (simplified)
		max_reflect = max_reflect_buffer;
	else
	{
		max_reflect_buffer = max_reflect;
		max_reflect = 1;
	}

	simplified = !simplified;
}

void Render::choose_samples_amount(const WindowProp& window_prop)
{
	if (simplified)
		samples_per_frame = 1;
	else if (rendering)
	{
		bool correct = false;
		samples_per_frame = MAX_SAMPLES_PER_FRAME;
		do
		{
			if (render_samples - window_prop.render_frame < samples_per_frame)
				samples_per_frame /= 2;
			else
				correct = true;
		} while (!correct);
	}
	else if (window_prop.focus)
		if (window_prop.fixed_frame_counter < 30)
			samples_per_frame = 1;
		else
			samples_per_frame = viewport_samples;
	else
		samples_per_frame = 1;
}

void Render::StartRender(const Ini& setup)
{
	if (simplified)
		switch_image_quality();

	int width_counter = ceil(float(setup.w) / MAX_CLASTER_SIZE);
	int height_counter = ceil(float(setup.h) / MAX_CLASTER_SIZE);
	clasters = vector<vector<ImageClaster>>(height_counter, vector<ImageClaster>(width_counter, ImageClaster()));

	for (int y = 0; y < height_counter; y++)
		for (int x = 0; x < width_counter; x++)
		{
			Vector2i block_position = Vector2i(x * MAX_CLASTER_SIZE, y * MAX_CLASTER_SIZE);
			Vector2i block_size = Vector2i
			(
				x + 1 != width_counter ? MAX_CLASTER_SIZE : setup.w - (width_counter - 1) * MAX_CLASTER_SIZE,
				y + 1 != height_counter ? MAX_CLASTER_SIZE : setup.h - (height_counter - 1) * MAX_CLASTER_SIZE
			);
			clasters[y][x].Create(block_position, block_size);
		}
	claster_pointer = &clasters[0][0];

	cout << clasters.size() << '\n'
		<<height_counter <<'\n'
		<< ceil(float(setup.w) / MAX_CLASTER_SIZE) << '\n';
	rendering = true;
}

void Render::render_claster()
{

}

void Render::set_uniforms(Shader& shader, const WindowProp& window_prop, const Ini& setup, const Camera& camera, const Texture& lol)
{
	shader.setUniform("resolution", Vector2f(setup.w, setup.h));
	shader.setUniform("light_dir", setup.light_dir);

	shader.setUniform("frame", window_prop.frame);
	shader.setUniform("preFrame", window_prop.preFrame);
	shader.setUniform("fixed_frame_counter", window_prop.fixed_frame_counter);

	shader.setUniform("camera_origin", camera.camera_origin);
	shader.setUniform("camera_rotation", camera.camera_rotation);
	shader.setUniform("focal_length", (float)camera.focal_length);
	shader.setUniform("aperture", (float)camera.aperture);
	shader.setUniform("camera_size", (float)camera.camera_size);

	this->choose_samples_amount(window_prop);
	shader.setUniform("render", this->rendering);
	shader.setUniform("samples", this->samples_per_frame);
	shader.setUniform("sun_size", this->sun_size);
	shader.setUniform("max_reflect", this->max_reflect);

	vector<Vector3f> seeds(samples_per_frame, Vector3f(0, 0, 0));
	for (int i = 0; i < samples_per_frame; i++)
		seeds[i] = Vector3f(
			(rand() % 100000),
			(rand() % 100000),
			(rand() % 100000)
		);

	shader.setUniformArray("seeds", &seeds[0], samples_per_frame);
}

bool Render::save_result(const ImageAccurate& render_dump, const Clock& render_elapsed_time, const Ini& setup)
{
	auto elapsed_time = FormatTime(render_elapsed_time.getElapsedTime());
	cout << "Render done!\n";
	cout << "Elapsed time to render : " << TimeToString(elapsed_time) << "\n";

	int count = 0;
	for (const auto& file : filesystem::directory_iterator(setup.render_path))
		count++;
	string file_name = setup.render_path + to_string(count) + '_' + TimeToString(elapsed_time, '_', true) + ".png";

	cout << "try to save: " << file_name << "\n";

	Image render_output = Graphic::VectorToImage(render_dump, setup);

	if (render_output.saveToFile(file_name))
	{
		cout << "Saved: " + file_name + "\n";
		return true;
	}
	else
		cout << "Can't save to this path: " + file_name + "\n";

	return false;
}