#version 130

uniform sampler2D image;
uniform vec2 resolution;
float aspect_ratio = resolution.x/resolution.y;

uniform int frame;
uniform int fixed_frame_counter;
uniform vec2 rand_compare;

uniform sampler2D preFrame;

uniform vec3 camere_origin;
uniform vec3 camere_rotation;

uniform vec3 light_dir;

uniform int max_reflect;
uniform int samples;

uniform sampler2D sky;

void main()
{
    vec2 uv = gl_FragCoord.xy/resolution;
    vec3 color_curr = texture(image, uv).rgb;
    float pow_r = pow(color_curr.r, 2); 
    pow_r = color_curr.r;
    gl_FragColor = vec4(pow_r, color_curr.gb, 1);
}