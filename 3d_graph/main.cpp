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
#include "ini.h"

using namespace sf;
using namespace std;
using ImageAccurate = vector<vector<Vector3<double>>>;

int main()
{
	srand(time(NULL));
#pragma region window

	ContextSettings con_set;
	con_set.antialiasingLevel = 8;
	RenderWindow window(VideoMode(w, h), "bebe", Style::Default, con_set);
	window.setFramerateLimit(120);
#pragma endregion

#pragma region shader
	Shader shader;
	if (!shader.loadFromFile("Shader.frag", Shader::Type::Fragment)) //shadertest
		cout << "ERROR: LOAD SHADER\n";
	shader.setUniform("resolution", Vector2f(w, h));
	RectangleShape filler(Vector2f(window.getSize()));
	filler.setFillColor(Color::Cyan);
#pragma endregion	

#pragma region sky
	Texture sky;
	if (!sky.loadFromFile("sky.jpg"))
		cout << "ERROR: LOAD SKY\n";
	shader.setUniform("sky", sky);
#pragma endregion

	shader.setUniform("max_reflect", max_reflect);
	shader.setUniform("sun_size", sun_size);

	Clock FrameTime;
	int frame = 0, render_frame = 0, fixed_frame_counter = 1;
	int current_samples = 0;

	ImageAccurate render_dump(h, vector<Vector3<double>>(w, Vector3<double>(0,0,0)));

	bool render = false;
	bool focus = false;

	Texture preFrame;
	preFrame.create(w, h);

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
				if (camera.RotateCamera(event, window))
					fixed_frame_counter = 1;
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::R)
				{
					cout << "Start render:\n";
					render = true;
					camera.Disable();
				}

				if (event.key.code == Keyboard::Escape)
				{
					focus = false;
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
				current_samples = viewport_samples;
		else
			current_samples = 1;

		shader.setUniform("render", render);
		shader.setUniform("samples", current_samples);

		shader.setUniform("seed", Vector2f(rand(), rand()));

		shader.setUniform("camera_origin", camera.camera_origin);
		shader.setUniform("camera_rotation", camera.camera_rotation);

		shader.setUniform("light_dir", light_dir);

		shader.setUniform("preFrame", preFrame);
#pragma endregion

		window.draw(filler, &shader);
		window.display();
		preFrame.update(window);

		if (render)
		{
			Graphic::RenderApproximate(render_dump, preFrame.copyToImage(), render_samples, w, h);
			render_frame++;
			cout << render_frame << " sample--\n";

			if (render_frame == render_samples)
			{
				cout << "Render done!\n";

				int count = 0;
				for (const auto& file : filesystem::directory_iterator(render_path))
					count++;
				string file_name = render_path + to_string(count) + ".png"; // !!!!!!!!!!!!!!fix elapsed time

				cout << "try to save: " << file_name << "\n";

				Image render_output = Graphic::VectorToImage(render_dump, w, h);

				if (render_output.saveToFile(file_name))		//current state of [preFrame] is last render
					cout << "Saved: " + file_name + "\n";
				else
					cout << "Can't save to this path: " + file_name + "\n";

				render_frame = 0;
				fixed_frame_counter = 1;
				render = false;
				camera.Enable();
			}
		}

		frame++;
		FrameTime.restart();
	}

	return 0;
}