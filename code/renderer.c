#include "renderer.h"
#include "file.h"

#define VERT_FILEPATH "./shaders/simple.vert"

const char* fragment_filepaths[SHADER_COUNT] = {
	[SHADER_COLOR] = "./shaders/color.frag",
	[SHADER_IMAGE] = "./shaders/image.frag",
	[SHADER_TEXT] = "./shaders/text.frag",
};

static inline bool
ShaderCompile(const GLchar* source, GLenum shader_type, GLuint* shader)
{
	*shader = glCreateShader(shader_type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLchar message[1024];
        GLsizei message_size = 0;
        glGetShaderInfoLog(*shader, sizeof(message), &message_size, message);
        fprintf(stderr,
				"ERROR: could not compile %s\n",
				shader_type == GL_VERTEX_SHADER ?
				"GL_VERTEX_SHADER" :
				"GL_FRAGMENT_SHADER");
        fprintf(stderr, "%.*s\n", message_size, message);
        return false;
    }

    return true;
}

static inline void
ShaderCompileFile(const char* filepath, GLenum shader_type, GLuint* shader)
{
	char* file;
	file = ReadFile(filepath);
	ShaderCompile(file, shader_type, shader);
	free(file);
}

static inline void
ShaderAttachToProgram(GLuint* shaders, usize count, GLuint program)
{
	for(usize i = 0; i < count; i++)
		glAttachShader(program, shaders[i]);
}

static inline void
ProgramLink(GLuint program, const char* fp, usize line)
{
	glLinkProgram(program);

	GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLsizei message_size = 0;
        GLchar message[1024];

        glGetProgramInfoLog(program, sizeof(message), &message_size, message);
        fprintf(stderr, "%s:%zu: Program Linking: %.*s\n", fp, line, message_size, message);
    }
}

typedef struct {
	Uniform slot;
	const char* name;
} UniformPair;

static const UniformPair uniform_pairs[UNIFORM_COUNT] = {
	[UNIFORM_TIME] = {
		.slot = UNIFORM_TIME,
		.name = "time"
	},
	[UNIFORM_RESOLUTION] = {
		.slot = UNIFORM_RESOLUTION,
		.name = "resolution"
	},
	[UNIFORM_CAMERA_POS] = {
		.slot = UNIFORM_CAMERA_POS,
		.name = "camera_pos"
	},
	[UNIFORM_CAMERA_SCALE] = {
		.slot = UNIFORM_CAMERA_SCALE,
		.name = "camera_scale"
	},
};

static inline void
GetUniformLocation(GLuint program, GLint locations[UNIFORM_COUNT])
{
	for(Uniform u = 0; u < UNIFORM_COUNT; u++) {
		locations[u] = glGetUniformLocation(program, uniform_pairs[u].name);
	}
}

void
RendererInit(Renderer* renderer)
{
	// renderer->camera_scale = 3.0f;
	renderer->camera_scale = 1.0f;

    {
        glGenVertexArrays(1, &renderer->vao);
        glBindVertexArray(renderer->vao);

        glGenBuffers(1, &renderer->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->verticies), renderer->verticies, GL_DYNAMIC_DRAW);

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (GLvoid *) offsetof(Vertex, position));

        // color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (GLvoid *) offsetof(Vertex, color));

        // uv
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (GLvoid *) offsetof(Vertex, uv));
    }

	GLuint shaders[2] = {0};

	ShaderCompileFile(VERT_FILEPATH, GL_VERTEX_SHADER, shaders);

	for(uint i = 0; i < SHADER_COUNT; i++) {
		ShaderCompileFile(fragment_filepaths[i], GL_FRAGMENT_SHADER, &shaders[1]);
		renderer->programs[i] = glCreateProgram();
		ShaderAttachToProgram(shaders, sizeof(shaders) / sizeof(shaders[0]), renderer->programs[i]);
		ProgramLink(renderer->programs[i], __FILE__, __LINE__);
		glDeleteShader(shaders[1]);
	}
	glDeleteShader(shaders[0]);
}

void
RendererVertex(Renderer* renderer, vec2 pos, vec4 col, vec2 uv)
{
	Vertex* last = &renderer->verticies[renderer->verticies_count];
	last->position = pos;
	last->color = col;
	last->uv = uv;
	renderer->verticies_count += 1;
}

void
RendererTriangle(Renderer* renderer,
				 vec2 p0, vec2 p1, vec2 p2,
				 vec4 c0, vec4 c1, vec4 c2,
				 vec2 uv0, vec2 uv1, vec2 uv2)
{
	RendererVertex(renderer, p0, c0, uv0);
    RendererVertex(renderer, p1, c1, uv1);
    RendererVertex(renderer, p2, c2, uv2);
}

void
RendererQuad(Renderer* renderer,
			 vec2 p0, vec2 p1, vec2 p2, vec2 p3,
			 vec4 c0, vec4 c1, vec4 c2, vec4 c3,
			 vec2 uv0, vec2 uv1, vec2 uv2, vec2 uv3)
{
	RendererTriangle(renderer, p0, p1, p2, c0, c1, c2, uv0, uv1, uv2);
    RendererTriangle(renderer, p1, p2, p3, c1, c2, c3, uv1, uv2, uv3);
}

void
RendererRect(Renderer* renderer, vec2 p, vec2 s, vec4 c)
{
	vec2 uv = {0};
	RendererQuad(renderer,
				 p, Vec2Add(p, Vec2Init(s.x, 0)), Vec2Add(p, Vec2Init(0, s.y)), Vec2Add(p, s),
				 c,c,c,c,
				 uv,uv,uv,uv);
}

void
RendererImageRect(Renderer* renderer, vec2 p, vec2 s, vec2 uvp, vec2 uvs, vec4 c)
{
	RendererQuad(renderer,
				 p, Vec2Add(p, Vec2Init(s.x, 0)), Vec2Add(p, Vec2Init(0, s.y)), Vec2Add(p, s),
				 c,c,c,c,
				 uvp, Vec2Add(uvp, Vec2Init(uvs.x, 0)), Vec2Add(uvp, Vec2Init(0, uvs.y)), Vec2Add(uvp, uvs));
}

void
RendererFlush(Renderer* renderer)
{
	RendererSync(renderer);
	RendererDraw(renderer);
	renderer->verticies_count = 0;
}

void
RendererSync(Renderer* renderer)
{
	glBufferSubData(GL_ARRAY_BUFFER,
					0,
					renderer->verticies_count * sizeof(Vertex),
					renderer->verticies);
}

void
RendererDraw(Renderer* renderer)
{
	glDrawArrays(GL_TRIANGLES, 0, renderer->verticies_count);
}

void
RendererSetShader(Renderer* renderer, Shader shader)
{
	renderer->current_shader = shader;
	glUseProgram(renderer->programs[renderer->current_shader]);
	GetUniformLocation(renderer->programs[renderer->current_shader], renderer->uniforms);

	glUniform2f(renderer->uniforms[UNIFORM_RESOLUTION], renderer->resolution.x, renderer->resolution.y);
    glUniform1f(renderer->uniforms[UNIFORM_TIME], renderer->time);
    glUniform2f(renderer->uniforms[UNIFORM_CAMERA_POS], renderer->camera_pos.x, renderer->camera_pos.y);
    glUniform1f(renderer->uniforms[UNIFORM_CAMERA_SCALE], renderer->camera_scale);
}
