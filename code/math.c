#include "math.h"

vec2
vec2_init(float x, float y)
{
	return (vec2) { .x = x, .y = y };
}

vec2
vec2_init_s(float x)
{
	return (vec2) { .x = x, .y = x };
}

vec2
vec2_add(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x + b.x, .y = a.y + b.y };
}

vec2
vec2_sub(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x - b.x, .y = a.y - b.y };
}

vec2
vec2_mul(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x * b.x, .y = a.y * b.y };
}

vec2
vec2_div(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x / b.x, .y = a.y / b.y };
}

vec2
vec2_mul3(vec2 a, vec2 b, vec2 c)
{
	return vec2_mul(vec2_mul(a, b), c);
}


vec4
vec4_init(float x, float y, float z, float w)
{
	return (vec4) { .x = x, .y = y, .z = z, .w = w };
}

vec4
vec4_init_s(float x)
{
	return (vec4) { .x = x, .y = x, .z = x, .w = x };
}

vec4
vec4_add(vec4 a, vec4 b)
{
	return vec4_init(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

vec4
vec4_sub(vec4 a, vec4 b)
{
	return vec4_init(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

vec4
vec4_mul(vec4 a, vec4 b)
{
	return vec4_init(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

vec4
vec4_div(vec4 a, vec4 b)
{
	return vec4_init(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}
