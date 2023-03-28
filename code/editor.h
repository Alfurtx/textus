#ifndef EDITOR_H
#define EDITOR_H

#include "common.h"
#include "renderer.h"
#include "arena.h"
#include "glyph.h"
#include "file.h"

typedef struct {
	usize begin;
	usize end;
} Line;

// NOTE(fonsi): maybe directly integrate it in the EditorState struct instead
// of being a completly new struct
typedef struct {
	usize capacity;
	usize count;
	Line* items;
} Lines;

typedef struct {
	vec2 cursor_info;

	Arena* arena;
	CharacterAtlas* atlas;

	usize cursor_row;
	usize cursor_col;

	Buffer data;
	Lines lines;
} EditorState;

void editor_init(EditorState* e, Arena* arena, CharacterAtlas* atlas);
void editor_render(EditorState* e, Renderer* renderer, GLFWwindow* window);
void editor_load_file(EditorState* e, const char* filepath);
void editor_move_cursor_right(EditorState* e);
void editor_move_cursor_left(EditorState* e);

#endif
