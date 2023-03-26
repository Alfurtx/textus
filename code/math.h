#ifndef MATH_H
#define MATH_H

typedef struct vec2 {
	float x, y;
} vec2;

vec2 Vec2Init(float x, float y);
vec2 Vec2Inits(float x);
vec2 Vec2Add(vec2 a, vec2 b);
vec2 Vec2Sub(vec2 a, vec2 b);
vec2 Vec2Mul(vec2 a, vec2 b);
vec2 Vec2Div(vec2 a, vec2 b);
vec2 Vec2Mul3(vec2 a, vec2 b, vec2 c);

typedef struct vec4 {
	float x, y, z, w;
} vec4;

vec4 Vec4Init(float x, float y, float z, float w);
vec4 Vec4Inits(float x);
vec4 Vec4Add(vec4 a, vec4 b);
vec4 Vec4Sub(vec4 a, vec4 b);
vec4 Vec4Mul(vec4 a, vec4 b);
vec4 Vec4Div(vec4 a, vec4 b);

float lerpf(float a, float b, float t);

#endif
