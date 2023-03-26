#ifndef RENDERER_H
#define RENDERER_H

#include "common.h"
#include "arena.h"
#include "math.h"
#include "glyph.h"

typedef uint Shader;

Shader ShaderCreateProgram(Arena* arena, char* vertex_file_path, char* fragment_file_path);
void   ShaderUseProgram(Shader handle);
void   ShaderSetUniformVec2(Shader handle, const char* name, vec2 vec);
void   ShaderSetUniformVec4(Shader handle, const char* name, vec4 vec);

void RenderText(Shader shader, const char* text, float x, float y, float scale);

#endif
