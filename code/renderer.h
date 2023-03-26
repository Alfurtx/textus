#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "arena.h"

typedef uint Shader;

Shader ShaderCreateProgram(Arena* arena, char* vertex_file_path, char* fragment_file_path);
void   ShaderUseProgram(Shader handle);

#endif
