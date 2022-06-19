#include "Ini.h"

Ini::Ini(int w, int h, Vector3f light_dir, int sun_size, int max_reflect, int render_samples,
	int viewport_samples, string render_path)
	: w(w), h(h), light_dir(Graphic::Normalize(light_dir)), sun_size(sun_size), max_reflect(max_reflect),
	render_samples(render_samples), viewport_samples(viewport_samples), render_path(render_path)
{
	Font font;
	if (!font.loadFromFile("Arial.ttf"))
	{
		std::cout << "Didn't find font: Arial.ttf\n";
		return;
	}
	Text setup_t;
	setup_t.setCharacterSize(24);
	setup_t.setFont(font);
	setup_t.setFillColor(Color::Black);
	setup_t.setString(
		"W: " + to_string(w) + "; H: " + to_string(h) + "\n"
		+ "Maximum reflect: " + to_string(max_reflect) + "\n"
		+ "Viewport samples: " + to_string(viewport_samples) + "\n"
		+ "Render samples: " + to_string(render_samples) + "\n"
		+ "Viewport samples: " + to_string(viewport_samples)
	);

	info_text.insert({ "setup_t", setup_t });

	Text render_t;
	render_t.setCharacterSize(24);
	render_t.setFont(font);
	render_t.setFillColor(Color::Black);
	render_t.setString("");

	info_text.insert({ "render_t", render_t });
}

bool Ini::Draw(RenderWindow& window)
{
	if (disable)
		return false;

	for (auto const& [key, val] : info_text)
		window.draw(val);

	window.display();
	return true;
}
