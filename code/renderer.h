#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "arena.h"
#include "math.h"

typedef struct {
	vec2 position;
	vec2 uv;
	vec4 color;
} Vertex;

#define VERTICES_CAPACITY (3 * 640 * 1000)

typedef enum {
	UNIFORM_TIME = 0,
	UNIFORM_RESOLUTION,
	UNIFORM_CAMERA_POS,
	UNIFORM_CAMERA_SCALE,
	UNIFORM_COUNT,
} Uniform;

typedef enum {
	SHADER_COLOR = 0,
	SHADER_IMAGE,
	SHADER_TEXT,
	SHADER_COUNT,
} Shader;

typedef struct {
	GLuint vao;
	GLuint vbo;
	GLuint programs[SHADER_COUNT];
	Shader current_shader;

	GLint uniforms[UNIFORM_COUNT];
	Vertex verticies[VERTICES_CAPACITY];
	usize verticies_count;

	vec2 camera_pos;
	vec2 resolution;
	float time;
} Renderer;

void renderer_init(Renderer* renderer);
void renderer_set_shader(Renderer* renderer, Shader shader);
void renderer_flush(Renderer* renderer);
void renderer_sync(Renderer* renderer);
void renderer_draw(Renderer* renderer);

void renderer_push_vertex(Renderer* renderer, vec2 pos, vec4 col, vec2 uv);
void renderer_push_triangle(Renderer* renderer,
							vec2 p0,  vec2 p1,  vec2 p2,
							vec4 c0,  vec4 c1,  vec4 c2,
							vec2 uv0, vec2 uv1, vec2 uv2);
void renderer_push_quad(Renderer* renderer,
						vec2 p0,  vec2 p1,  vec2 p2,  vec2 p3,
						vec4 c0,  vec4 c1,  vec4 c2,  vec4 c3,
						vec2 uv0, vec2 uv1, vec2 uv2, vec2 uv3);
void renderer_push_rect(Renderer* renderer, vec2 p, vec2 s, vec4 c);
void renderer_push_image_rect(Renderer* renderer, vec2 p, vec2 s, vec2 uvp, vec2 uvs, vec4 c);

#endif
