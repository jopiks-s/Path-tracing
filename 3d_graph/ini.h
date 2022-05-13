#include <SFML/Graphics.hpp>
#include <cmath>
#include "Graphic.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#pragma once

using namespace sf;

extern int w, h;
extern float sensitivity;

extern Vector3f camere_origin;
extern Vector3f camere_rotation;
extern float camera_speed;

extern Vector3f light_dir;

extern int max_reflect;
extern int samples;
extern int viewport_samples;