#include <SFML/Graphics.hpp>
#include "Graphic.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "ini.h"
using namespace sf;

int main()
{
	srand(time(NULL));
	#pragma region window
	
		ContextSettings con_set;
		con_set.antialiasingLevel = 8;
		RenderWindow window(VideoMode(w, h), "bebe", Style::Default, con_set);
		window.setFramerateLimit(120);
		window.setMouseCursorVisible(false);
	#pragma endregion

		std::string render_path = "D:\\Ainstall\\render\\1.png";
		//std::cout << "Render path: ";
		//std::cin >> render_path;

	#pragma region shader
		Shader shader;
		if (!shader.loadFromFile("Shader.frag", Shader::Type::Fragment)) //shadertest
			std::cout << "ERROR: LOAD SHADER\n";
		shader.setUniform("resolution", Vector2f(w, h));
		RectangleShape filler(Vector2f(window.getSize()));
		filler.setFillColor(Color::Cyan);
	#pragma endregion	

	//#pragma region sky
	//	sf::Texture sky;
	//	if (!sky.loadFromFile("sky.jpg"))
	//		std::cout << "ERROR: LOAD SKY\n";
	//	shader.setUniform("sky", sky);
	//#pragma endregion

	shader.setUniform("max_reflect", max_reflect);

	Clock FrameTime;
	int frame = 0;
	int fixed_frame_counter = 1;
	int current_samples = 1;

	bool render = false;
	bool focus = true;

	Texture preFrame;
	preFrame.create(w, h);

	Vector3f fly_dir(0, 0, 0);

	while (window.isOpen())
	{
		if(frame%2 == 1)
			fixed_frame_counter++;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::LostFocus)
			{
				focus = false;
				window.setMouseCursorVisible(true);
			}
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
			{
				focus = true;
				window.setMouseCursorVisible(false);
			}
				
			if (event.type == Event::MouseMoved && focus)
			{
				float difx = ((event.mouseMove.x - w / 2) / (float)w) * sensitivity;
				float dify = ((event.mouseMove.y - h / 2) / (float)h) * sensitivity;
				camere_rotation.z += difx;
				camere_rotation.y += dify;
				Mouse::setPosition(Vector2i(w / 2, h / 2), window);
				if(difx != 0 || dify != 0)
					fixed_frame_counter = 1;
			}
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::R)
					render = true;

				if (event.key.code == Keyboard::Escape)
				{
					focus = false;
					window.setMouseCursorVisible(true);
				}
				if (!focus)
					continue;
				if (event.key.code == Keyboard::W)
					fly_dir.x = 1;
				if (event.key.code == Keyboard::S)
					fly_dir.x = -1;
				if (event.key.code == Keyboard::A)
					fly_dir.y = -1;
				if (event.key.code == Keyboard::D)
					fly_dir.y = 1;
				if (event.key.code == Keyboard::Space)
					fly_dir.z = 1;
				if (event.key.code == Keyboard::LShift)
					fly_dir.z = -1;
			}
			if (event.type == Event::KeyReleased)
			{
				if (event.key.code == Keyboard::W)
					fly_dir.x = 0;
				if (event.key.code == Keyboard::S)
					fly_dir.x = 0;
				if (event.key.code == Keyboard::A)
					fly_dir.y = 0;
				if (event.key.code == Keyboard::D)
					fly_dir.y = 0;
				if (event.key.code == Keyboard::Space)
					fly_dir.z = 0;
				if (event.key.code == Keyboard::LShift)
					fly_dir.z = 0;
			}
		}

		//render = true;
#pragma region camera movement
		if (fly_dir.x != 0 || fly_dir.y != 0 || fly_dir.z != 0)
			fixed_frame_counter = 1;
			
		int z_buff = fly_dir.z;
		camere_origin.z += fly_dir.z * camera_speed;
		fly_dir.z = 0;
		Vector3f buff_v = Graphic::Normalize(fly_dir);
		buff_v = Graphic::Rotate(buff_v, camere_rotation);
		camere_origin.x += buff_v.x * camera_speed;
		camere_origin.y += buff_v.y * camera_speed;
		camere_origin.z += buff_v.z * camera_speed;
		fly_dir.z = z_buff;
#pragma endregion

#pragma region shader unifroms
		shader.setUniform("frame", frame);
		shader.setUniform("fixed_frame_counter", fixed_frame_counter);

		if (render)
			current_samples = samples;
		else if (fixed_frame_counter < 10)
			current_samples = 1;
		else
			current_samples = viewport_samples;

		shader.setUniform("samples", current_samples);

		Vector2f rand_compare(rand()%5000-2500, rand()%5000-2500);
		shader.setUniform("rand_compare", rand_compare);

		shader.setUniform("camere_origin", camere_origin);
		shader.setUniform("camere_rotation", camere_rotation);

		shader.setUniform("light_dir", light_dir);
		
		shader.setUniform("preFrame", preFrame);
#pragma endregion
		window.draw(filler, &shader);

		window.display();
		preFrame.update(window);

		if (render)
		{
			preFrame.copyToImage().saveToFile(render_path);
			fixed_frame_counter = 1;
			render = false;
		}
		
		std::cout << "rendered frame: " + std::to_string(frame)+"\n";
		frame++;
		FrameTime.restart();
	}

	return 0;
}