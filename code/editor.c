#include "editor.h"

void
editor_init(EditorState* e, Arena* arena, CharacterAtlas* atlas)
{
	e->arena = arena;
	e->atlas = atlas;
}

void
editor_render(EditorState* e, Renderer* renderer, GLFWwindow* window)
{
	// render cursor
	renderer_set_shader(renderer, SHADER_COLOR);
	vec2 cursorinfo = get_cursor_pos_and_width(e->atlas, e->data.items, e->data.count, Vec2Inits(.0f), e->cursor_col);
	vec2 cpos = { .x = cursorinfo.x, .y = 0.0f };
	vec2 csize = Vec2Init(cursorinfo.y, FONT_SIZE);
	vec4 ccolor = Vec4Init(.0f, 1.0f, .0f, 1.0f);

	renderer_push_rect(renderer, cpos, csize, ccolor);
	renderer_flush(renderer);

	// render text
	renderer_set_shader(renderer, SHADER_TEXT);
	vec2 textpos = Vec2Inits(.0f);
	character_atlas_render_line(e->atlas, renderer, e->data.items, e->data.count, &textpos, Vec4Inits(1.0f));
	renderer_flush(renderer);
}

void
editor_load_file(EditorState* e, const char* filepath)
{
	// TODO
	e->data.items = "Hola Mundo => ?";
	e->data.count = strlen(e->data.items);
	e->data.capacity = strlen(e->data.items);
}

void
editor_move_cursor_right(EditorState* e)
{
	if(e->cursor_col < e->data.count)
		e->cursor_col++;
}

void
editor_move_cursor_left(EditorState* e)
{
	if(e->cursor_col > 0)
		e->cursor_col--;
}
