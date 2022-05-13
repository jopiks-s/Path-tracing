#include "ini.h"

int w = 1200;
int h = 800;

float sensitivity = 0.5;
float camera_speed = 0.3;

Vector3f light_dir = Graphic::Normalize(Vector3f(0.6, 0.75, -1.0));

int max_reflect = 32;
int samples = 1024;
int viewport_samples = 1;

Vector3f camere_origin(10, -4, 5);
Vector3f camere_rotation(0, 0,3.14);
