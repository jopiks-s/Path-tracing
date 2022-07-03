#include "Render.h"
#include "Ini.h"
#include "sfml_extension.h"
#include "WindowProp.h"
#include "Camera.h"

Render::Render(int viewport_samples, int render_samples, int MAX_CLASTER, int sun_size, int max_reflect)
	: viewport_samples(viewport_samples), render_samples(render_samples), MAX_CLASTER(MAX_CLASTER > 256 ? 256 : MAX_CLASTER),
	sun_size(sun_size), max_reflect(max_reflect)
{
	if (MAX_CLASTER > 256)
		cout << "GPU CAN'T HANDLE THIS CLASTER_SIZE: " << MAX_CLASTER << '\n'
		<< "MAX_CLASTER SET TO 256 \n"
		<< "To change max claster on gpu go to 'Shader.frag', line: 10, and change array size \n";
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

void Render::choose_claster_size(const WindowProp& window_prop)
{
	if (simplified)
		claster_size = 1;
	else if (rendering)
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
		if (window_prop.fixed_frame_counter < 30)
			claster_size = 1;
		else
			claster_size = viewport_samples;
	else
		claster_size = 1;
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

	shader.setUniform("render", this->rendering);
	shader.setUniform("samples", this->claster_size);
	shader.setUniform("sun_size", this->sun_size);
	shader.setUniform("max_reflect", this->max_reflect);

	vector<Vector3f> seeds(claster_size, Vector3f(0, 0, 0));
	for (int i = 0; i < claster_size; i++)
		seeds[i] = Vector3f(
			(rand() % 100000),
			(rand() % 100000),
			(rand() % 100000)
		);

	shader.setUniformArray("seeds", &seeds[0], claster_size);
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