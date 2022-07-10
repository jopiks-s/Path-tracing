#include "Render.h"
#include "Ini.h"
#include "sfml_extension.h"
#include "WindowProp.h"
#include "Camera.h"

Render::Render(int viewport_samples, int render_samples, int MAX_SAMPLES_PER_FRAME, int MAX_CLASTER_SIZE, int sun_size, int max_reflect, const Ini& setup)
	: viewport_samples(viewport_samples), render_samples(render_samples),
	MAX_SAMPLES_PER_FRAME(MAX_SAMPLES_PER_FRAME > 516 ? 516 : MAX_SAMPLES_PER_FRAME),
	MAX_CLASTER_SIZE(MAX_CLASTER_SIZE),
	render_dump(setup.h, vector<Vector3<long double>>(setup.w, Vector3<long double>(0, 0, 0))),
	sun_size(sun_size), max_reflect(max_reflect)
{
	if (MAX_SAMPLES_PER_FRAME > 516)
		cout << "GPU CAN'T HANDLE THIS SAMPLES PER FRAME: " << MAX_SAMPLES_PER_FRAME << '\n'
		<< "SAMPLES_PER_FRAME SET TO 516 \n"
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
		int samples_left = render_samples - image_clasters.samples_counter;
		if (samples_left > MAX_SAMPLES_PER_FRAME)
			samples_per_frame = MAX_SAMPLES_PER_FRAME;
		else
			samples_per_frame = samples_left;
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

	image_clasters.create_clasters(*this, setup);
	render_dump = ImageAccurate(setup.h, vector<Vector3<long double>>(setup.w, Vector3<long double>(0, 0, 0)));
	rendering = true;
}

void Render::EndRender()
{
	rendering = false;
}

bool Render::render_claster(RenderWindow& window, const Shader& shader, WindowProp& window_prop, const Ini& setup)
{
	if (!rendering)
		return false;

	window.draw(image_clasters.claster, &shader);
	window_prop.preFrame.update(window);
	Graphic::RenderApproximate(render_dump, window_prop.preFrame.copyToImage(), setup, *this);

	//window.draw((&image_clasters)->claster, &shader);
	image_clasters.samples_counter += samples_per_frame;

	if (image_clasters.samples_counter > render_samples)
		throw "Rendered too much samples";

	if (image_clasters.samples_counter != render_samples)
	{
		if (ssaved == 0)
		{
			window_prop.preFrame.copyToImage().saveToFile("D:/AInstall/beiii.png");
			ssaved++;
		}
	}
	else
	{
		if (image_clasters.IsLast())
		{
			EndRender();
			return true;
		}

		image_clasters.NextClaster();
	}
	return false;
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
	shader.setUniform("rendering", this->rendering);
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

bool Render::save_result(const Clock& render_elapsed_time, const Ini& setup)
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