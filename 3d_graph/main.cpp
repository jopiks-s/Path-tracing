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

	Render render(1, 256, 8, 256, 8, 32);
	Ini setup(500, 500, Vector3f(0.5, 0.75, -0.35), "D:/Ainstall/render/");
	WindowProp window_prop(setup.w, setup.h);
	InfoOutput info_output("Arial.ttf", Color::White, true);
	Camera camera(0.5, 0.3, 1, 620, 1, Vector3f(-30,0,7.5), Vector3f(0,-8,0));
	ImageAccurate render_dump(setup.h, vector<Vector3<long double>>(setup.w, Vector3<long double>(0, 0, 0)));

	Shader shader;
	if (!shader.loadFromFile("Shader.frag", Shader::Type::Fragment))
		cout << "Can't load 'Shader.frag'\n";

	////
	RectangleShape filler(Vector2f(setup.w, setup.h));
	filler.setFillColor(Color::Magenta);
	////

#pragma region window
	RenderWindow window(VideoMode(setup.w, setup.h), "bebe", window_prop.resizable ? Style::Default : Style::Titlebar | Style::Close);
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
				window_prop.disable_fullscrean(window, setup);
				window_prop.focus = false;
				camera.Disable();

				window.setMouseCursorVisible(true);
			}
			if (event.type == Event::MouseButtonPressed ||
				event.type == Event::GainedFocus)
			{
				window_prop.focus = true;
				window_prop.updated = true;

				window.setMouseCursorVisible(false);

				if (!render.rendering)
					camera.Enable();
			}
			if (event.type == Event::Resized)
			{
				window_prop.enable_fullscrean(window, setup);
				window_prop.focus = true;
				window_prop.updated = true;

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
					if (event.key.code == Keyboard::Enter)
						window_prop.enable_fullscrean(window, setup);
					if (event.key.code == Keyboard::Escape)
					{
						window_prop.disable_fullscrean(window, setup);
						window_prop.focus = false;
						camera.Disable();
						window.setMouseCursorVisible(true);
					}

					if (event.key.code == Keyboard::I)
						info_output.Switch();

					if (!render.rendering)
					{
						if (event.key.code == Keyboard::R)
						{
							cout << "Start render:\n";
							render.StartRender(setup);
							window_prop.render_elapsed_time.restart();
							camera.Disable();
							render_dump = ImageAccurate(setup.h, vector<Vector3<long double>>(setup.w, Vector3<long double>(0, 0, 0)));
						}

						if (event.key.code == Keyboard::Z)
							render.switch_image_quality();
					}
				}
				if (camera.KeyboardInputRecord(event))
					window_prop.fixed_frame_counter = 1;
			}
		}

		camera.MoveCamera();

		render.set_uniforms(shader, window_prop, setup, camera, sky);

		//render.render_claster();
		window.draw(filler, &shader);
		window_prop.preFrame.update(window);

		info_output.draw(window, setup, camera, render);

		if (!window_prop.updated)
		{
			draw_img(window, Graphic::VectorToImage(render_dump, setup));
			info_output.draw_render_done(window, setup);
		}

		if (render.rendering)
		{
			if (!info_output.disable)
				draw_img(window, Graphic::VectorToImage(render_dump, setup));

			window_prop.render_frame += render.samples_per_frame;
			info_output.draw_render(window, setup, window_prop.render_frame, window_prop.render_elapsed_time, render);

			Graphic::RenderApproximate(render_dump, window_prop.preFrame.copyToImage(), setup, render);

			if (window_prop.render_frame == render.render_samples)
			{
				render.save_result(render_dump, window_prop.render_elapsed_time, setup);

				window_prop.render_frame = 0;
				window_prop.fixed_frame_counter = 1;
				if (!window_prop.focus)
					window_prop.updated = false;
				render.rendering = false;
				camera.Enable();
			}
		}

		window.display();
		window_prop.frame++;
		window_prop.calculate_fps(window, "bebe");
	}

	return 0;
}