#include "InfoOutput.h"
#include "Ini.h"
#include "Graphic.h"
#include "sfml_extension.h"
#include "Camera.h"
#include "Render.h"

InfoOutput::InfoOutput(string font_path, Color font_color, bool disable) : font_color(font_color), disable(disable)
{
	font = Font();
	if (!font.loadFromFile(font_path))
	{
		std::cout << "Didn't find font: " << font_path << "\n";
		disable = true;
		return;
	}
}

void InfoOutput::Disable()
{
	disable = true;
}

void InfoOutput::Enable()
{
	disable = false;
}

void InfoOutput::Switch()
{
	disable = !disable;
}

bool InfoOutput::draw(RenderWindow& window, const Ini& setup, const Camera& camera, const Render& render)
{
	if (disable)
		return false;
	if (render.rendering)
		return false;

	auto angle = Graphic::ValueToAngle(camera.camera_rotation);

	Text setup_t;
	setup_t.setCharacterSize(24);
	setup_t.setFont(font);
	setup_t.setFillColor(font_color);
	ostringstream output_str;
	output_str <<
		"W : " << (setup.w) << "; H : " << (setup.h) << "\n"
		<< "Maximum reflect : " << render.max_reflect << "\n"
		<< "Viewport samples : " << render.viewport_samples << "\n"
		<< "Render samples : " << render.render_samples << "\n"
		<< "Render path : \"" << setup.render_path << "\"" << "\n"
		<< "Camera position : x: "
		<< ((int)camera.camera_origin.x) << "; y: "
		<< ((int)camera.camera_origin.y) << "; z: "
		<< ((int)camera.camera_origin.z) << ";" << "\n"
		<< "Camera rotation : "
		<< ((int)angle.x) << "* "
		<< ((int)angle.y) << "* "
		<< ((int)angle.z) << "*" << "\n"
		<< "Camera focal lenght : " << camera.focal_length << "\n"
		<< "Camera aperture : f/" << camera.aperture << "\n"
		<< "Claster size : " << render.samples_per_frame;
	setup_t.setString(output_str.str());
	setup_t.setPosition(Vector2f(10, 10));

	window.draw(setup_t);

	return true;
}

bool InfoOutput::draw_render(RenderWindow& window, const Ini& setup, int sample, const Clock& elapsed_time, const Render& render)
{
	if (disable)
		return false;

	auto time_str = TimeToString(FormatTime(elapsed_time.getElapsedTime()));

	ostringstream output_str;
	output_str <<
		sample << " sample--\n" <<
		"Elapsed time : " << time_str << "\n" <<
		"Claster size : " << render.samples_per_frame << "\n";
	cout << output_str.str();
	Text render_t;
	render_t.setCharacterSize(24);
	render_t.setFont(font);
	render_t.setFillColor(font_color);
	render_t.setString(output_str.str());
	render_t.setOrigin(render_t.getGlobalBounds().width, 0);
	render_t.setPosition(setup.w - 10, 10);

	window.draw(render_t);

	return true;
}

bool InfoOutput::draw_render_done(RenderWindow& window, const Ini& setup)
{
	Text render_done_t;
	render_done_t.setCharacterSize(56);
	render_done_t.setFont(font);
	render_done_t.setFillColor(font_color);
	render_done_t.setString("Render done!");
	render_done_t.setOrigin(render_done_t.getLocalBounds().width / 2, render_done_t.getLocalBounds().height / 2);
	render_done_t.setPosition(setup.w / 2, setup.h / 2);

	window.draw(render_done_t);

	return true;
}