//#include <SFML/Graphics.hpp>
//#include "Graphic.h"
//#include <cmath>
//#include <iostream>
//#include <stdlib.h>
//#include <time.h>
//#include <algorithm>
//#include <chrono>
//#include <cstdio>
//#include <fstream>
//#include <filesystem>
//#include "ini.h"
//using namespace sf;
//using namespace std;
//
//int main()
//{
//	srand(time(NULL));
//#pragma region window
//
//	ContextSettings con_set;
//	con_set.antialiasingLevel = 8;
//	RenderWindow window(VideoMode(w, h), "bebe", Style::Default, con_set);
//	window.setFramerateLimit(120);
//#pragma endregion
//
//#pragma region shader
//	Shader shader;
//	if (!shader.loadFromFile("Debug.frag", Shader::Type::Fragment)) //shadertest
//		cout << "ERROR: LOAD SHADER\n";
//	RectangleShape filler(Vector2f(window.getSize()));
//	filler.setFillColor(Color::Cyan);
//#pragma endregion
//
//	bool load = false;
//	int load_count = 0;
//
//	while (window.isOpen())
//	{
//		Event event;
//		while (window.pollEvent(event))
//		{
//			if (event.type == Event::Closed)
//				window.close();
//			
//			if (event.type == Event::KeyPressed)
//			{
//				if (event.key.code == Keyboard::R)
//				{
//					load = true;
//					cout << "Load start\n";
//				}
//
//				if (event.key.code == Keyboard::Escape)
//					window.close();
//			}
//		}
//		shader.setUniform("load", load);
//		shader.setUniform("load_count", load_count);
//		
//		window.draw(filler, &shader);
//		window.display();
//
//		if (load)
//		{
//			load = false;
//			load_count++;
//			cout << "Load stop\n";
//		}
//	}
//
//	return 0;
//}