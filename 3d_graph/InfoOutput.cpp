#include "InfoOutput.h"
#include "Ini.h"
#include "Graphic.h"

InfoOutput::InfoOutput(string font_path, bool disable) : disable(disable)
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

bool InfoOutput::draw(RenderWindow& window, const Ini& setup)
{
	if (disable)
		return false;

	Text setup_t;
	setup_t.setCharacterSize(24);
	setup_t.setFont(font);
	setup_t.setFillColor(Color::White);
	setup_t.setString(
		"W : " + to_string(setup.w) + "; H : " + to_string(setup.h) + "\n"
		+ "Maximum reflect : " + to_string(setup.max_reflect) + "\n"
		+ "Viewport samples : " + to_string(setup.viewport_samples) + "\n"
		+ "Render samples : " + to_string(setup.render_samples) + "\n"
		+ "Render path : \"" + setup.render_path + "\""
		+ "\n"
		+ "\n"
		+ "Movement: ^[W] <[A] v[S] >[D] ^^[SPACE] vv[LSHIFT]" + "\n"
		+ "To start render: [R]" + "\n"
		+ "To close info: [I]"
	);
	setup_t.setPosition(Vector2f(10, 10));

	window.draw(setup_t);
	window.display();

	return true;
}

bool InfoOutput::render_draw(RenderWindow& window, const Ini& setup, int sample, const Clock& elapsed_time)
{
	if (disable)
		return false;

	auto time_str = Graphic::TimeToString(Graphic::FormatTime(elapsed_time.getElapsedTime()));
	cout << sample << " sample--\n";
	cout << "Elapsed time : " << time_str << "\n";
	Text render_t;
	render_t.setCharacterSize(24);
	render_t.setFont(font);
	render_t.setFillColor(Color::White);
	render_t.setString(
		"Rendered samples : " + to_string(sample) + "\n"
		+ "Elapsed time : " + time_str
	);
	render_t.setOrigin(render_t.getGlobalBounds().width, 0);
	render_t.setPosition(setup.w - 10, 10);

	window.draw(render_t);
	window.display();

	return true;
}
