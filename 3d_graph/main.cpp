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

	Ini setup(1200, 800, Vector3f(0.6, 0.75, -1.0), 8, 32, 128, 16, "D:/Ainstall/render/");
	WindowProp window_prop(setup.w, setup.h);
	InfoOutput info_output("Arial.ttf");
	Camera camera(0.5, 0.3, 1.5, 1, Vector3f(0, 10, 5), Vector3f(0, 0, 0));
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

				if (!window_prop.render)
					camera.Enable();
			}

			if (event.type == Event::MouseMoved && window_prop.focus && !window_prop.render)
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
					window_prop.render = true;
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
			camera.KeyboardInputRecord(event);
		}

		if (camera.MoveCamera())
			window_prop.fixed_frame_counter = 1;

#pragma region shader unifroms
		shader.setUniform("frame", window_prop.frame);
		shader.setUniform("fixed_frame_counter", window_prop.fixed_frame_counter);

		if (window_prop.render)
			window_prop.current_samples = 1;
		else if (window_prop.focus)
			if (window_prop.fixed_frame_counter < 10)
				window_prop.current_samples = 1;
			else
				window_prop.current_samples = setup.viewport_samples;
		else
			window_prop.current_samples = 1;

		shader.setUniform("render", window_prop.render);
		shader.setUniform("samples", window_prop.current_samples);

		shader.setUniform("seed", Vector2f(rand(), rand()));

		shader.setUniform("camera_origin", camera.camera_origin);
		shader.setUniform("camera_rotation", camera.camera_rotation);
		shader.setUniform("focal_distance", (float)camera.focal_distance);
		shader.setUniform("camera_size", (float)camera.camera_size);

		shader.setUniform("light_dir", setup.light_dir);

		shader.setUniform("preFrame", window_prop.preFrame);
#pragma endregion

		window.draw(filler, &shader);
		window_prop.preFrame.update(window);

		info_output.draw(window, setup, camera);

		if (window_prop.render)
		{
			if (!info_output.disable)
				draw_img(window, Graphic::VectorToImage(render_dump, setup));

			window_prop.render_frame++;
			info_output.render_draw(window, setup, window_prop.render_frame, window_prop.render_elapsed_time);

			Graphic::RenderApproximate(render_dump, window_prop.preFrame.copyToImage(), setup);

			if (window_prop.render_frame == setup.render_samples)
			{
				Render::save_result(render_dump, window_prop.render_elapsed_time, setup);

				window_prop.render_frame = 0;
				window_prop.fixed_frame_counter = 1;
				window_prop.render = false;
				camera.Enable();
				render_dump = ImageAccurate(setup.h, vector<Vector3<long double>>(setup.w, Vector3<long double>(0, 0, 0)));
			}
		}

		window.display();
		window_prop.frame++;
	}

	return 0;
}