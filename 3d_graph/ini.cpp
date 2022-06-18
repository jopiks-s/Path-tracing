#include "ini.h"

int w = 1200;
int h = 800;

Camera camera(0.5, 0.3, Vector3f(0, 10, 5), Vector3f(0, 0, 0));

Vector3f light_dir = Graphic::Normalize(Vector3f(0.6, 0.75, -1.0));
int sun_size = 8;

int max_reflect = 32;
int render_samples = 64;
int viewport_samples = 4;

std::string render_path = "D:\\Ainstall\\render\\";