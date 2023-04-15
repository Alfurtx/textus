#include "renderer.h"
#include "file.h"

#define VERT_FILEPATH "./shaders/simple.vert"

const char* fragment_filepaths[SHADER_COUNT] = {
    [SHADER_COLOR] = "./shaders/color.frag",
    [SHADER_IMAGE] = "./shaders/image.frag",
    [SHADER_TEXT]  = "./shaders/text.frag",
};

static inline void
shader_compile_file(const char* filepath, GLenum shader_type, GLuint* shader)
{
	Buffer file = {0};
    read_file(filepath, &file);
	file.items[file.count] = '\0';
	{
		*shader = glCreateShader(shader_type);
		glShaderSource(*shader, 1, &file.items, NULL);
		glCompileShader(*shader);

		GLint compiled = 0;
		glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

		if(!compiled) {
			GLchar  message[1024];
			GLsizei message_size = 0;
			glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
			fprintf(stderr,
					"ERROR: could not compile %s\n",
					shader_type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER"
					: "GL_FRAGMENT_SHADER");
			fprintf(stderr, "%.*s\n", message_size, message);
		}

	}
	Assert(file.items != NULL, "file.items IS NULL");
    free(file.items);
}

static inline void
link_program(GLuint program, const char* fp, usize line)
{
    glLinkProgram(program);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(!linked) {
        GLsizei message_size = 0;
        GLchar  message[1024];

        glGetProgramInfoLog(program, sizeof(message), &message_size, message);
        fprintf(stderr,
                "%s:%zu: Program Linking: %.*s\n",
                fp,
                line,
                message_size,
                message);
    }
}

typedef struct {
    Uniform slot;
    const char* name;
} UniformPair;

static const UniformPair uniform_pairs[UNIFORM_COUNT] = {
    [UNIFORM_TIME]         = {.slot = UNIFORM_TIME,         .name = "time"},
    [UNIFORM_RESOLUTION]   = {.slot = UNIFORM_RESOLUTION,   .name = "resolution"},
    [UNIFORM_CAMERA_POS]   = {.slot = UNIFORM_CAMERA_POS,   .name = "camera_pos"},
    [UNIFORM_CAMERA_SCALE] = {.slot = UNIFORM_CAMERA_SCALE, .name = "camera_scale"},
};

void
renderer_init(Renderer* renderer)
{
    {
        glGenVertexArrays(1, &renderer->vao);
        glBindVertexArray(renderer->vao);

        glGenBuffers(1, &renderer->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(renderer->verticies),
                     renderer->verticies,
                     GL_DYNAMIC_DRAW);

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (GLvoid*)offsetof(Vertex, position));

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (GLvoid*)offsetof(Vertex, color));

        // uv
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (GLvoid*)offsetof(Vertex, uv));
    }

    GLuint shaders[2] = {0};

    shader_compile_file(VERT_FILEPATH, GL_VERTEX_SHADER, shaders);

    for(uint i = 0; i < SHADER_COUNT; i++) {
        shader_compile_file(fragment_filepaths[i], GL_FRAGMENT_SHADER, &shaders[1]);
        renderer->programs[i] = glCreateProgram();

		for(uint s = 0; s < ARRAY_COUNT(shaders); s++) {
			glAttachShader(renderer->programs[i], shaders[s]);
		}

        link_program(renderer->programs[i], __FILE__, __LINE__);
        glDeleteShader(shaders[1]);
    }
    glDeleteShader(shaders[0]);
}

void
renderer_push_vertex(Renderer* renderer, vec2 pos, vec4 col, vec2 uv)
{
    Vertex* last   = &renderer->verticies[renderer->verticies_count];
    last->position = pos;
    last->color    = col;
    last->uv       = uv;
    renderer->verticies_count += 1;
}

void
renderer_push_triangle(Renderer* renderer,
					   vec2 p0,  vec2 p1,  vec2 p2,
					   vec4 c0,  vec4 c1,  vec4 c2,
					   vec2 uv0, vec2 uv1, vec2 uv2)
{
    renderer_push_vertex(renderer, p0, c0, uv0);
    renderer_push_vertex(renderer, p1, c1, uv1);
    renderer_push_vertex(renderer, p2, c2, uv2);
}

void
renderer_push_quad(Renderer* renderer,
				   vec2 p0,  vec2 p1,  vec2 p2,  vec2 p3,
				   vec4 c0,  vec4 c1,  vec4 c2,  vec4 c3,
				   vec2 uv0, vec2 uv1, vec2 uv2, vec2 uv3)
{
    renderer_push_triangle(renderer, p0, p1, p2, c0, c1, c2, uv0, uv1, uv2);
    renderer_push_triangle(renderer, p1, p2, p3, c1, c2, c3, uv1, uv2, uv3);
}

void
renderer_push_rect(Renderer* renderer, vec2 p, vec2 s, vec4 c)
{
    vec2 uv = {0};
    renderer_push_quad(renderer,
					   p, vec2_add(p, vec2_init(s.x, 0)), vec2_add(p, vec2_init(0, s.y)), vec2_add(p, s),
					   c,c,c,c,
					   uv,uv,uv,uv);
}

void
renderer_push_image_rect(Renderer* renderer, vec2 p, vec2 s, vec2 uvp, vec2 uvs, vec4 c)
{
    renderer_push_quad(renderer,
                 p, vec2_add(p, vec2_init(s.x, 0)), vec2_add(p, vec2_init(0, s.y)), vec2_add(p, s),
                 c, c, c, c,
                 uvp, vec2_add(uvp, vec2_init(uvs.x, 0)), vec2_add(uvp, vec2_init(0, uvs.y)), vec2_add(uvp, uvs));
}

void
renderer_flush(Renderer* renderer)
{
    renderer_sync(renderer);
    renderer_draw(renderer);
    renderer->verticies_count = 0;
}

void
renderer_sync(Renderer* renderer)
{
    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    renderer->verticies_count * sizeof(Vertex),
                    renderer->verticies);
}

void
renderer_draw(Renderer* renderer)
{
    glDrawArrays(GL_TRIANGLES, 0, renderer->verticies_count);
}

void
renderer_set_shader(Renderer* renderer, Shader shader)
{
    renderer->current_shader = shader;
    glUseProgram(renderer->programs[renderer->current_shader]);
	for(Uniform u = 0; u < UNIFORM_COUNT; u++) {
		renderer->uniforms[u] = glGetUniformLocation(renderer->programs[renderer->current_shader], uniform_pairs[u].name);
	}

    glUniform2f(renderer->uniforms[UNIFORM_RESOLUTION], renderer->resolution.x, renderer->resolution.y);
    glUniform1f(renderer->uniforms[UNIFORM_TIME], renderer->time);
    glUniform2f(renderer->uniforms[UNIFORM_CAMERA_POS], renderer->camera_pos.x, renderer->camera_pos.y);
    // glUniform1f(renderer->uniforms[UNIFORM_CAMERA_SCALE], renderer->camera_scale);
}
