#include "renderer.h"
#include "file.h"

Shader
ShaderCreateProgram(Arena* arena, char* vertex_file_path, char* fragment_file_path)
{
	ArenaLocal arena_local = ArenaGetScratch(arena);

	uint vert, frag;
	int success;
	char infoLog[512];

	vert = glCreateShader(GL_VERTEX_SHADER);
	char* vsrc = ReadFile(arena_local.arena, vertex_file_path);
	glShaderSource(vert, 1, &vsrc, 0);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vert, 512, NULL, infoLog);
		fprintf(stderr, "[ERROR][VERTEX SHADER COMPILATION ERROR]\n%s\n", infoLog);
	}

	frag = glCreateShader(GL_VERTEX_SHADER);
	char* fsrc = ReadFile(arena_local.arena, fragment_file_path);
	glShaderSource(frag, 1, &fsrc, 0);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		fprintf(stderr, "[ERROR][FRAGMENT SHADER COMPILATION ERROR]\n%s\n", infoLog);
	}

	Shader handle = glCreateProgram();
	glAttachShader(handle, vert);
	glAttachShader(handle, frag);
	glLinkProgram(handle);
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(frag, 512, NULL, infoLog);
		fprintf(stderr, "[ERROR][SHADER LINKING ERROR]\n%s\n", infoLog);
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	ArenaReleaseScratch(&arena_local);

	return 0;
}

void
ShaderUseProgram(Shader handle)
{
	glUseProgram(handle);
}
