#include "math.h"

vec2
Vec2Init(float x, float y)
{
	return (vec2) { .x = x, .y = y };
}

vec2
Vec2Inits(float x)
{
	return (vec2) { .x = x, .y = x };
}

vec2
Vec2Add(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x + b.x, .y = a.y + b.y };
}

vec2
Vec2Sub(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x - b.x, .y = a.y - b.y };
}

vec2
Vec2Mul(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x * b.x, .y = a.y * b.y };
}

vec2
Vec2Div(vec2 a, vec2 b)
{
	return (vec2) { .x = a.x / b.x, .y = a.y / b.y };
}

vec2
Vec2Mul3(vec2 a, vec2 b, vec2 c)
{
	return Vec2Mul(Vec2Mul(a, b), c);
}


vec4
Vec4Init(float x, float y, float z, float w)
{
	return (vec4) { .x = x, .y = y, .z = z, .w = w };
}

vec4
Vec4Inits(float x)
{
	return (vec4) { .x = x, .y = x, .z = x, .w = x };
}

vec4
Vec4Add(vec4 a, vec4 b)
{
	return Vec4Init(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

vec4
Vec4Sub(vec4 a, vec4 b)
{
	return Vec4Init(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

vec4
Vec4Mul(vec4 a, vec4 b)
{
	return Vec4Init(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

vec4
Vec4Div(vec4 a, vec4 b)
{
	return Vec4Init(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}
