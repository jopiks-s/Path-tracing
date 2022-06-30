uniform vec2 resolution;
float aspect_ratio = resolution.x / resolution.y;

uniform int frame;
uniform int fixed_frame_counter;
uniform bool render;

uniform vec2 seed;

uniform vec3 camera_origin;
uniform vec3 camera_rotation;
uniform float focal_length;
uniform float aperture;
uniform float camera_size;

uniform vec3 light_dir;
uniform int sun_size;

uniform sampler2D preFrame;
uniform int max_reflect;
uniform int samples;

uniform sampler2D sky;

///////////////////////////////////////////////
const int object_amount = 7;
int rand_counter = 0;

float Length(in vec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
float Length(in vec2 v) { return sqrt(v.x * v.x + v.y * v.y); }

vec3 Normalize(in vec3 v) { float l = Length(v);    return vec3(v.x / l, v.y / l, v.z / l); }
vec2 Normalize(in vec2 v) { float l = Length(v);    return vec2(v.x / l, v.y / l); }

float clamp(in float val, in float minv = 0, in float maxv = 1) { return max(minv, min(val, maxv)); }

vec3 clamp(in vec3 vec, in float minv = 0, in float maxv = 1) { return vec3(clamp(vec.x, minv, maxv), clamp(vec.y, minv, maxv), clamp(vec.z, minv, maxv)); }


float random_f(in vec2 uv, in vec3 ro, in vec3 rd, in bool full_range = true)
{
	ro = vec3(int(ro.x) % 10000, int(ro.y) % 10000, int(ro.z) % 3);
	rd = Normalize(rd);
	rand_counter += seed.x - seed.y;
	float rand_v = fract(sin(rand_counter + frame * ro.z + dot(rd.xy * uv, vec2(12.9898, 78.233) * seed * rd.z)) * (43758.5453123 + ro.x + ro.y));
	if(full_range)
	{
		rand_v *= 2;
		rand_v -= 1;
	}
	return rand_v;
}
vec3 random_v3(in vec2 uv, in vec3 ro, in vec3 rd, bool full_range = true, bool normalized = true)
{
	bool correct = false;
	vec3 ret;
	while (!correct)
	{
		ret = vec3(random_f(uv, ro, rd, full_range), random_f(uv, ro, rd, full_range), random_f(uv, ro, rd, full_range));
		if (Length(ret) < 1.0)
			correct = true;
	}

	if (normalized)
		return Normalize(ret);
	return ret;
}
vec2 random_v2(in vec2 uv, in vec3 ro, in vec3 rd, bool full_range = true, bool normalized = true)
{
	bool correct = false;
	vec2 ret;
	while (!correct)
	{
		ret = vec2(random_f(uv, ro, rd, full_range), random_f(uv, ro, rd, full_range));
		if (Length(ret) < 1.0)
			correct = true;
	}

	if (normalized)
		return Normalize(ret);
	return ret;
}

vec3 color_correction(in vec3 col) { return clamp(vec3(pow(col.r, 0.45), pow(col.g, 0.45), pow(col.b, 0.45))); }

vec3 Rotate(in vec3 v, in vec3 fi)
{
	vec3 ret = v;
	vec3 buf = ret;

	//rotate on y axis
	ret.x = buf.x * cos(fi.y) + buf.z * sin(fi.y);
	ret.y = buf.y;
	ret.z = -buf.x * sin(fi.y) + buf.z * cos(fi.y);

	buf = ret;

	//rotate on z axis
	ret.x = cos(fi.z) * buf.x - buf.y * sin(fi.z);
	ret.y = sin(fi.z) * buf.x + buf.y * cos(fi.z);

	return ret;
}

vec3 GetSky(in vec3 rd, in int sun_index = 0)
{
	if (sun_index == 0)
	{
		vec3 sky_col = color_correction(vec3(0.3, 0.6, 1.0));
		vec3 sun_col = color_correction(vec3(0.6353, 0.5333, 0.4902));
		sun_col *= pow(clamp(-dot(rd, light_dir)), pow(2, sun_size));
		return clamp(sky_col * 0.5 + sun_col);
	}
	if (sun_index == 1)
	{
		vec2 sky_uv = vec2(atan(rd.x, rd.y), asin(-rd.z) * 2);
		sky_uv /= 3.14;
		sky_uv = sky_uv * 0.5 + 0.5;
		vec3 sky_col = texture(sky, sky_uv).rgb;
		vec3 sun_col = color_correction(vec3(1, 1, 0.8));
		sun_col *= pow(clamp(-dot(rd, light_dir)), 256 / 2);
		return sky_col + sun_col;
	}
}

//c - circle

vec2 sphere_intersect(in vec3 ro, in vec3 rd, in vec3 co, float radius)
{
	vec3 oc = ro - co;
	float a = dot(rd, rd);
	float b = 2.0 * dot(oc, rd);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return vec2(-1.0);
	}
	b /= (2.0 * a);
	float h = sqrt(discriminant) / (2.0 * a);
	return vec2(-b - h, -b + h);
}

//p - plane

float plane_intersect(in vec3 ro, in vec3 rd, in vec3 po, in vec3 pn)
{
	pn = Normalize(pn);
	return dot(po - ro, pn) / dot(rd, pn);
}

vec2 box_intersect(in vec3 ro, in vec3 rd, in vec3 bo, in vec3 bs, out vec3 outN)
{
	vec3 oc = ro - bo;
	vec3 m = 1.0 / rd;
	vec3 n = m * oc;
	vec3 k = abs(m) * bs;
	vec3 t1 = -n - k;
	vec3 t2 = -n + k;

	float tN = max(max(t1.x, t1.y), t1.z);
	float tF = min(min(t2.x, t2.y), t2.z);

	if (tN > tF || tF < 0.0) return vec2(-1.0); // no intersection

	outN = -sign(rd) * step(t1.yzx, t1.xyz) * step(t1.zxy, t1.xyz);

	return vec2(tN, tF);
}

struct Material
{
	int type;
	vec3 color;
	float Roughness;
	float Specular;
	float Refraction;
};

struct Object
{
	int object_type;
	// 0 - sphere, 1 - box, 2 - plane

	vec3 pos;
	float size;
	Material mat;
	vec3 normal;
	vec2 distance;

	void color_ini() { mat.color = color_correction(mat.color); }

	void CalculateObjectGraphic(in vec3 ro, in vec3 rd)
	{
		if (object_type == 0)
		{
			distance = sphere_intersect(ro, rd, pos, size);
			normal = Normalize((ro + rd * distance.x) - pos);
		}
		if (object_type == 1)
			distance = box_intersect(ro, rd, pos, vec3(size), normal);
		if (object_type == 2)
			distance = vec2(plane_intersect(ro, rd, pos, normal));
	}
};

Object[object_amount] world_intersection(in Object obj[object_amount], in vec3 ro, in vec3 rd, out int minInd)
{
	Object buff[object_amount] = obj;
	minInd = -1;
	float min = 999999999;

	for (int i = 0; i < buff.length(); i++)
	{
		buff[i].CalculateObjectGraphic(ro, rd);
		if (buff[i].distance.x < min && buff[i].distance.x>0.00005)
		{
			min = buff[i].distance.x;
			minInd = i;
		}
	}
	return buff;
}

vec3 castRay(in vec3 ro, in vec3 rd, in Object obj[object_amount], in vec2 uv)
{
	rd = Normalize(rd);

	int min;
	vec3 color = vec3(1.0);

	for (int i = 0; i < max_reflect; i++)
	{
		if(Length(color) <= 0.01)
			return color;
		obj = world_intersection(obj, ro, rd, min);

		if (min == -1)
			return color * GetSky(rd);

		if (obj[min].mat.type == 1)
			return obj[min].mat.color;

		color *= obj[min].mat.color;

		float random = random_f(uv, ro, rd, false);
		float reflect_chance;
		if (obj[min].mat.Refraction == 1)
			reflect_chance = 100;
		else
			reflect_chance = 1 - clamp(-dot(rd, obj[min].normal));
		if (random < reflect_chance + obj[min].mat.Specular)		// then reflect
		{
			ro += rd * obj[min].distance.x + obj[min].normal*0.001; // obj[min].normal*0.001 - fix artefacts

			vec3 random_rd = random_v3(uv, ro, rd);
			random_rd = Normalize(random_rd * dot(obj[min].normal, random_rd));

			vec3 specular_rd = reflect(rd, obj[min].normal);

			rd = Normalize(mix(specular_rd, random_rd, obj[min].mat.Roughness));
		}
		else													   // else refract
		{
			vec3 roY_far = ro + rd * (obj[min].distance.y + 10);
			ro += rd * obj[min].distance.y + obj[min].normal*0.001;

			vec3 refract_rd = refract(rd, obj[min].normal, obj[min].mat.Refraction);

			obj[min].CalculateObjectGraphic(roY_far, -refract_rd);
			vec3 random_rd = random_v3(uv, ro, rd);
			random_rd = Normalize(random_rd * dot(obj[min].normal, random_rd));

			rd = Normalize(mix(refract_rd, random_rd, obj[min].mat.Roughness));
		}
	}

	return color;
}

vec3 MultiTrace(in vec2 uv)
{
	Object obj[object_amount];
	obj[0] = Object(2, vec3(0.0), 1, Material(0, vec3(0.3, 0.4, 0.1), 0.9, 0, 1), vec3(0, 0, 1), vec2(0.0));			//plane
	obj[1] = Object(0, vec3(3, 10, 0.5), 1.5, Material(0, vec3(1.0, 0.2, 0.2), 0.2, 0, 1), vec3(0.0), vec2(0.0));		//sphere
	obj[2] = Object(1, vec3(-5, 3, 4), 2.5, Material(0, vec3(0.3, 0.1, 0.3), 0.8, 0, 1), vec3(0.0), vec2(0.0));			//cub
	obj[3] = Object(1, vec3(-15, 7, 2), 2.5, Material(0, vec3(0.1, 1, 0), 0.7, 0, 1), vec3(0.0), vec2(0.0));			//cub
	obj[4] = Object(0, vec3(3, 5, 7), 2.5, Material(1, vec3(1, 1, 1), 1, 0, 1), vec3(0.0), vec2(0.0));					//lamp
	obj[5] = Object(0, vec3(-10, -3, 6.5), 2.5, Material(1, vec3(1, 1, 1), 1, 0, 1), vec3(0.0), vec2(0.0));				//lamp
	obj[6] = Object(0, vec3(10, 5, 2), 1.5, Material(0, vec3(0.7, 0.7, 0.7), 0, 0.1, 0.74), vec3(0.0), vec2(0.0));	//sphere
	for (int i = 0; i < obj.length(); i++)
		obj[i].color_ini();

	vec3 matrix_origin = vec3(0, -uv * camera_size);
	vec3 world_origin = camera_origin + Rotate(matrix_origin, camera_rotation);

	vec2 focus_coord = random_v2(uv, world_origin, Rotate(Normalize(vec3(1, uv)), camera_rotation), true, false) * (focal_length / aperture);

	vec3 rd = Rotate(vec3(focal_length, focus_coord) - matrix_origin, camera_rotation);

	vec3 col;
	for (int i = 0; i < samples; i++)
		col += castRay(world_origin, rd, obj, uv);
	return col / samples;
}

void main()
{
	vec2 uv = gl_FragCoord.xy / resolution - 0.5;
	uv.x *= aspect_ratio;

	vec3 curr_col = MultiTrace(uv);

	if (render)
		gl_FragColor = vec4(curr_col, 1);
	else
	{
		vec3 pre_col = texture(preFrame, gl_FragCoord.xy / resolution).rgb;	
		vec3 mix_col = mix(pre_col, curr_col, 1.0 / fixed_frame_counter);
		gl_FragColor = vec4(mix_col, 1);
	}
}