#ifndef MATH_H
#define MATH_H

typedef struct vec2 {
	float x, y;
} vec2;

vec2 vec2_init(float x, float y);
vec2 vec2_init_s(float x);
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 a, vec2 b);
vec2 vec2_div(vec2 a, vec2 b);
vec2 vec2_mul3(vec2 a, vec2 b, vec2 c);

typedef struct vec4 {
	float x, y, z, w;
} vec4;

vec4 vec4_init(float x, float y, float z, float w);
vec4 vec4_init_s(float x);
vec4 vec4_add(vec4 a, vec4 b);
vec4 vec4_sub(vec4 a, vec4 b);
vec4 vec4_mul(vec4 a, vec4 b);
vec4 vec4_div(vec4 a, vec4 b);

float lerpf(float a, float b, float t);

#endif
