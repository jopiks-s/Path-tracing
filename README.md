# Path-tracing

This is a project for the implementation of a type of computer graphics - path tracing.
The program has a camera object that controls POV, and primitives directly with which you can build a scene.

Inspired by [@ArtemOnigiri](https://github.com/ArtemOnigiri), from this [video](https://www.youtube.com/watch?v=jKjbeWHujV0)

# Suported primitives 📐

```c++
int object_type;
/*
0. sphere
1. box
2. plane
*/
```

# Suported materials 🏀
```c++
int type;
/*
0. default
1. lamp
*/
vec3 color;
float Roughness;  // shows how randomness will be next ray direction reflected or refract
float Specular;   // shows how likely ray will reflect, instead of refract (affects only if Refraction != 1)
float Refraction; // refraction property of object
```

# Movement
- ``` W,A,S,D ``` - move camera
- - ``` R ``` - start render
- ``` I ``` - enable/disable information output
- ``` Z ``` - simplify image quality
- ``` ESC ``` - detach program
- ``` ENTER ``` - enable fullscrean (works only if your window size is 1920x1080)
