#include "Render.h"
#include "Ini.h"
#include "sfml_extension.h"
#include "WindowProp.h"
#include "Camera.h"

void RenderF::set_uniforms(Shader& shader, const WindowProp& window_prop, const Ini& setup, const Render& render, const Camera& camera)
{
	shader.setUniform("light_dir", setup.light_dir);

	shader.setUniform("frame", window_prop.frame);
	shader.setUniform("preFrame", window_prop.preFrame);

	shader.setUniform("fixed_frame_counter", window_prop.fixed_frame_counter);
	shader.setUniform("render", render.rendering);
	shader.setUniform("samples", render.claster_size);

	shader.setUniform("camera_origin", camera.camera_origin);
	shader.setUniform("camera_rotation", camera.camera_rotation);
	shader.setUniform("focal_distance", (float)camera.focal_distance);
	shader.setUniform("camera_size", (float)camera.camera_size);

	shader.setUniform("seed", Vector2f(rand(), rand()));

}

bool RenderF::save_result(const ImageAccurate& render_dump, const Clock render_elapsed_time, const Ini& setup)
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