#include <SFML/Graphics.hpp>
#include "Graphic.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <bitset>
#include "Camera.h"
#include "Ini.h"
#include "InfoOutput.h"
#include "sfml_extension.h"
#include "Render.h"
#include "WindowProp.h"

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<long double>>>;

int main()
{
	srand(time(NULL));

	Render render(4, 2048, 64);
	Ini setup(1200, 800, Vector3f(0.6, 0.75, -1.0), 8, 32, "D:/Ainstall/render/");
	WindowProp window_prop(setup.w, setup.h);
	InfoOutput info_output("Arial.ttf", true);
	Camera camera(0.5, 0.3, 1, 620, 1, Vector3f(15, -41, 44), Vector3f(0, 42, 111));
	ImageAccurate render_dump(setup.h, vector<Vector3<long double>>(setup.w, Vector3<long double>(0, 0, 0)));

#pragma region shader
	Shader shader;
	if (!shader.loadFromFile("Shader.frag", Shader::Type::Fragment))
		cout << "Can't load Shader.frag\n";

	RectangleShape filler(Vector2f(setup.w, setup.h));
	filler.setFillColor(Color::Cyan);

	shader.setUniform("max_reflect", setup.max_reflect);
	shader.setUniform("sun_size", setup.sun_size);
	shader.setUniform("resolution", Vector2f(setup.w, setup.h));
#pragma endregion

#pragma region window

	ContextSettings con_set;
	con_set.antialiasingLevel = 8;
	RenderWindow window(VideoMode(setup.w, setup.h), "bebe", Style::Default, con_set);
	window.setFramerateLimit(120);
#pragma endregion

#pragma region sky
	Texture sky;
	if (!sky.loadFromFile("sky.jpg"))
		cout << "ERROR: LOAD SKY\n";
	shader.setUniform("sky", sky);
#pragma endregion

	while (window.isOpen())
	{
		window_prop.fixed_frame_counter++;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::LostFocus)
			{
				window_prop.focus = false;
				camera.Disable();
				window.setMouseCursorVisible(true);
			}
			if (event.type == Event::MouseButtonPressed)
			{
				window_prop.focus = true;
				window.setMouseCursorVisible(false);

				if (!render.rendering)
					camera.Enable();
			}

			if (window_prop.focus) //responso only if FOCUS
			{
				if (event.type == Event::MouseMoved && window_prop.focus && !render.rendering)
				{
					if (camera.RotateCamera(event, window, setup))
						window_prop.fixed_frame_counter = 1;
				}

				if (event.type == Event::KeyPressed)
				{
					if (!window_prop.focus)
						continue;
					if (event.key.code == Keyboard::R)
					{
						cout << "Start render:\n";
						render.rendering = true;
						window_prop.render_elapsed_time.restart();
						camera.Disable();
					}

					if (event.key.code == Keyboard::I)
						info_output.Switch();

					if (event.key.code == Keyboard::Escape)
					{
						window_prop.focus = false;
						camera.Disable();
						window.setMouseCursorVisible(true);
					}
				}
				if (camera.KeyboardInputRecord(event))
					window_prop.fixed_frame_counter = 1;
			}
		}

		camera.MoveCamera();

		render.choose_claster_size(window_prop);
		render.set_uniforms(shader, window_prop, setup, camera, sky);

		window.draw(filler, &shader);
		window_prop.preFrame.update(window);

		info_output.draw(window, setup, camera, render);

		if (render.rendering)
		{
			if (!info_output.disable)
				draw_img(window, Graphic::VectorToImage(render_dump, setup));

			window_prop.render_frame += render.claster_size;
			info_output.render_draw(window, setup, window_prop.render_frame, window_prop.render_elapsed_time, render);

			Graphic::RenderApproximate(render_dump, window_prop.preFrame.copyToImage(), setup, render);

			if (window_prop.render_frame == render.render_samples)
			{
				render.save_result(render_dump, window_prop.render_elapsed_time, setup);

				window_prop.render_frame = 0;
				window_prop.fixed_frame_counter = 1;
				render.rendering = false;
				camera.Enable();
				render_dump = ImageAccurate(setup.h, vector<Vector3<long double>>(setup.w, Vector3<long double>(0, 0, 0)));
			}
		}

		window.display();
		window_prop.frame++;
		window_prop.calculate_fps(window, "bebe");
	}

	return 0;
}