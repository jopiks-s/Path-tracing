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

	int w = 500, h = 500,
		viewport_samples = 1,
		render_samples = 1024, max_samples_per_frame = 128, max_claster_size = 64, sun_size = 8, max_reflect = 32;
	double sensetivity = 0.5, camera_speed = 0.3, focal_length = 1, aperture = 620, camera_size = 1;
	Vector3f light_dir(0.5, 0.75, -0.35),
		camera_origin(-30, 0, 7.5), camera_rotation(0, -8, 0); //degrees
	string render_path = "D:/AInstall/render/";

	Ini setup(w, h, light_dir, render_path);
	Render render(viewport_samples, render_samples, max_samples_per_frame, max_claster_size, sun_size, max_reflect, setup);
	WindowProp window_prop(setup.w, setup.h);
	InfoOutput info_output("Arial.ttf", Color::White, true);
	Camera camera(sensetivity, camera_speed, focal_length, aperture, camera_size, camera_origin, camera_rotation);

	Shader shader;
	if (!shader.loadFromFile("shadertest.frag", Shader::Type::Fragment))
		cout << "Can't load 'Shader.frag'\n";
	RectangleShape raw_filler(Vector2f(setup.w, setup.h));

#pragma region window
	RenderWindow window(VideoMode(setup.w, setup.h), "bebe", window_prop.resizable ? Style::Default : Style::Titlebar | Style::Close);
	window.setFramerateLimit(1);
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
			if (event.type == Event::MouseButtonPressed)
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
						info_output.Switch(window);

					if (!render.rendering)
					{
						if (event.key.code == Keyboard::R)
						{
							cout << "Start render:\n";
							render.StartRender(setup);
							window_prop.render_elapsed_time.restart();
							camera.Disable();
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

		if (render.rendering)
		{
			if (render.render_claster(window, shader, window_prop, setup))
			{
				render.save_result(window_prop.render_elapsed_time, setup);

				window_prop.fixed_frame_counter = 1;
				window_prop.updated = window_prop.focus;

				camera.Enable();
			}
			if (!info_output.disable)
				draw_img(window, Graphic::VectorToImage(render.render_dump, setup));
		}
		else
		{
			if (!window_prop.updated)
			{
				draw_img(window, Graphic::VectorToImage(render.render_dump, setup));
				info_output.draw_render_done(window, setup);
			}
			else
				window.draw(raw_filler, &shader);

			window_prop.preFrame.update(window);
		}

		info_output.draw(window, setup, camera, render);
		info_output.draw_render(window, setup, window_prop.render_elapsed_time, render);

		window.display();
		window_prop.frame++;
		window_prop.calculate_fps(window, "bebe");
	}

	getchar();
}