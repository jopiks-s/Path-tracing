#include <SFML/Graphics.hpp>
#include <cmath>
#include "Graphic.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include "Camera.h"

#pragma once

using namespace sf;

extern int w, h;

extern Camera camera;

extern Vector3f light_dir;
extern int sun_size;

extern int max_reflect;
extern int render_samples;
extern int viewport_samples;

extern std::string render_path;