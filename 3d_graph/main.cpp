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

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<long double>>>;

int main()
{
	srand(time(NULL));

	Ini setup(1200, 800, Vector3f(0.6, 0.75, -1.0), 8, 32, 128, 16, "D:/Ainstall/render/");
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

	Clock render_elapsed_time;
	int frame = 0, render_frame = 0, fixed_frame_counter = 1;
	int current_samples = 0;

	bool render = false,
		focus = false;

	Texture preFrame;
	preFrame.create(setup.w, setup.h);

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
		fixed_frame_counter++;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::LostFocus)
			{
				focus = false;
				camera.Disable();
				window.setMouseCursorVisible(true);
			}
			if (event.type == Event::MouseButtonPressed)
			{
				focus = true;
				window.setMouseCursorVisible(false);

				if (!render)
					camera.Enable();
			}

			if (event.type == Event::MouseMoved && focus && !render)
			{
				if (camera.RotateCamera(event, window, setup))
					fixed_frame_counter = 1;
			}

			if (event.type == Event::KeyPressed)
			{
				if (!focus)
					continue;
				if (event.key.code == Keyboard::R)
				{
					cout << "Start render:\n";
					render = true;
					render_elapsed_time.restart();
					camera.Disable();
				}

				if (event.key.code == Keyboard::I)
					info_output.Switch();

				if (event.key.code == Keyboard::Escape)
				{
					focus = false;
					camera.Disable();
					window.setMouseCursorVisible(true);
				}
			}
			camera.KeyboardInputRecord(event);
		}

		if (camera.MoveCamera())
			fixed_frame_counter = 1;

#pragma region shader unifroms
		shader.setUniform("frame", frame);
		shader.setUniform("fixed_frame_counter", fixed_frame_counter);

		if (render)
			current_samples = 1;
		else if (focus)
			if (fixed_frame_counter < 10)
				current_samples = 1;
			else
				current_samples = setup.viewport_samples;
		else
			current_samples = 1;

		shader.setUniform("render", render);
		shader.setUniform("samples", current_samples);

		shader.setUniform("seed", Vector2f(rand(), rand()));

		shader.setUniform("camera_origin", camera.camera_origin);
		shader.setUniform("camera_rotation", camera.camera_rotation);
		shader.setUniform("focal_distance", (float)camera.focal_distance);
		shader.setUniform("camera_size", (float)camera.camera_size);

		shader.setUniform("light_dir", setup.light_dir);

		shader.setUniform("preFrame", preFrame);
#pragma endregion

		window.draw(filler, &shader);
		preFrame.update(window);

		info_output.draw(window, setup, camera);

		if (render)
		{
			if (!info_output.disable)
				draw_img(window, Graphic::VectorToImage(render_dump, setup));

			render_frame++;
			info_output.render_draw(window, setup, render_frame, render_elapsed_time);

			Graphic::RenderApproximate(render_dump, preFrame.copyToImage(), setup);

			if (render_frame == setup.render_samples)
			{
				Render::save_result(render_dump, render_elapsed_time, setup);

				render_frame = 0;
				fixed_frame_counter = 1;
				render = false;
				camera.Enable();
				render_dump = ImageAccurate(setup.h, vector<Vector3<long double>>(setup.w, Vector3<long double>(0, 0, 0)));
			}
		}

		window.display();
		frame++;
	}

	return 0;
}