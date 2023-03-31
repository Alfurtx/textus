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
	GLFWwindow* window;

	usize cursor;
	usize scroll;
	usize linetop;
	usize linebot;
	Buffer filepath;

	Buffer data;
	Lines lines;
} EditorState;

// NOTE(fonsi): offset so that it stays centered on the text
#define CURSOR_OFFSET 0.13f

void editor_init(EditorState* e, Arena* arena, CharacterAtlas* atlas, GLFWwindow* w);
void editor_render(EditorState* e, Renderer* renderer);
void editor_load_file(EditorState* e, const char* filepath);
void editor_rebuild_lines(EditorState* e);
usize editor_get_cursor_row(EditorState* e);
vec2 editor_cursor_pos(EditorState* e);

void editor_move_cursor_right(EditorState* e);
void editor_move_cursor_left(EditorState* e);
void editor_move_cursor_up(EditorState* e);
void editor_move_cursor_down(EditorState* e);

void editor_scroll_up(EditorState* e);
void editor_scroll_down(EditorState* e);

void editor_insert_char(EditorState* e, char c);
void editor_insert_text(EditorState* e, Buffer text);

void editor_save_file(EditorState* e);

#endif
