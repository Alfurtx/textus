#include "editor.h"

void
editor_init(EditorState* e, CharacterAtlas* atlas, GLFWwindow* w)
{
	e->atlas = atlas;
	e->window = w;
}

usize
editor_get_cursor_row(EditorState* e)
{
	Assert(e->lines.count > 0, "");
	for(usize i = 0; i < e->lines.count; i++) {
		Line line = e->lines.items[i];
		if(e->cursor >= line.begin && e->cursor <= line.end)
			return i;
	}
	return e->lines.count - 1;
}

void
editor_render(EditorState* e, Renderer* renderer)
{
	/* Window Info */
	int w, h;
	glfwGetWindowSize(e->window, &w, &h);

	renderer->resolution = vec2_init(w, h);
	renderer->time = glfwGetTime();

	/* Cursor Update */
	vec2 cursor_pos = vec2_init_s(0.0f);
	float cursor_width = 0.0f;
	{
		usize crow = editor_get_cursor_row(e);
		Line l = e->lines.items[crow];
		usize ccol = e->cursor - l.begin;
		char* text = e->data.items + l.begin;
		cursor_pos.y = h - (crow + 1 + CURSOR_OFFSET) * FONT_SIZE;
		cursor_pos.x = char_atlas_cursor_pos(e->atlas, text, l.end - l.begin, ccol);
		cursor_width = char_atlas_cursor_width(e->atlas, text, l.end - l.begin, ccol);
		if(cursor_width == 0.0f) cursor_width = e->atlas->characters['A'].ax;
	}

	/* Cursor Render */
	{
		renderer_set_shader(renderer, SHADER_COLOR);
		renderer_push_rect(renderer, cursor_pos, vec2_init(cursor_width, FONT_SIZE), vec4_init(.0f, 1.0f, .0f, 1.0f));
		renderer_flush(renderer);
	}

	/* Text Render */
	{
		renderer_set_shader(renderer, SHADER_TEXT);
		for(usize i = 0; i < e->lines.count; i++) {
			vec2 tpos = vec2_init(.0f, h - ((i + 1) * FONT_SIZE));
			Line lpos = e->lines.items[i];
			char_atlas_render_line(e->atlas,
								   renderer,
								   &e->data.items[lpos.begin],
								   lpos.end-lpos.begin,
								   &tpos,
								   vec4_init_s(1.0f));
		}
		renderer_flush(renderer);
	}

	/* Camera Update */
	{
		vec2 target = cursor_pos;
		vec2 camvel = vec2_mul(vec2_sub(target, renderer->camera_pos), vec2_init_s(2.0));
		renderer->camera_pos = vec2_add(renderer->camera_pos, vec2_mul(camvel, vec2_init_s(DELTA_TIME)));
		// renderer->camera_pos = cursor_pos;
	}
}

void
editor_load_file(EditorState* e, const char* filepath)
{
	e->data.count = 0;
	read_file(filepath, &e->data);
	e->cursor = 0;
	editor_rebuild_lines(e);

	e->filepath.count = 0;
	arr_append_cstr(&e->filepath, filepath);
	arr_append_null(&e->filepath);

	int w, h; glfwGetWindowSize(e->window, &w, &h);

	return;
}

void
editor_rebuild_lines(EditorState* e)
{
	e->lines.count = 0;
	Line line;
	line.begin = 0;
	for(usize i = 0; i < e->data.count; i++) {
		if(e->data.items[i] == '\n') {
			line.end = i;
			arr_append(&e->lines, line);
			line.begin = i+1;
		}
	}
	line.end = e->data.count;
	arr_append(&e->lines, line);
}

void
editor_move_cursor_right(EditorState* e)
{
	if(e->cursor < e->data.count) {
			e->cursor += 1;
	}
}

void
editor_move_cursor_left(EditorState* e)
{
	if(e->cursor > 0) {
			e->cursor -= 1;
	}
}

void
editor_move_cursor_up(EditorState* e)
{
	usize crow = editor_get_cursor_row(e);
	if(crow > 0) {
		Line line = e->lines.items[crow];
		usize diff = e->cursor - line.begin;
		Line nextline = e->lines.items[crow - 1];
		if(nextline.begin + diff < nextline.end)
			e->cursor = nextline.begin + diff;
		else
			e->cursor = nextline.end;
	}
}

void
editor_move_cursor_down(EditorState* e)
{
	usize crow = editor_get_cursor_row(e);
	if(crow < e->lines.count - 1) {
		Line line = e->lines.items[crow];
		usize diff = e->cursor - line.begin;
		Line nextline = e->lines.items[crow + 1];
		if(nextline.begin + diff < nextline.end)
			e->cursor = nextline.begin + diff;
		else
			e->cursor = nextline.end;

	}
}

void
editor_insert_char(EditorState* e, char c)
{
	if(e->cursor > e->data.count)
		e->cursor = e->data.count;

	if(e->data.count >= e->data.capacity) {
		// TODO(fonsi): take into account empty files
		e->data.capacity *= 2;
		e->data.items = realloc(e->data.items, e->data.capacity * sizeof(*e->data.items));
		Assert(e->data.items, "Realloc FAILED");
	}

	arr_append(&e->data, '\0');

	memmove(&e->data.items[e->cursor + 1],
			&e->data.items[e->cursor],
			e->data.count - e->cursor - 1);

	memcpy(&e->data.items[e->cursor], &c, 1);
	e->cursor += 1;
	editor_rebuild_lines(e);
}

void
editor_save_file(EditorState* e)
{
	FILE* f = 0;
	f = fopen(e->filepath.items, "wb");
	Assert(f, "Failed to detect file to write");
	fwrite(e->data.items, 1, e->data.count, f);
	fclose(f);
}

void
editor_backspace(EditorState* e)
{
	if(e->cursor > e->data.count) e->cursor = e->data.count;
	if(e->cursor == 0) return;

	memmove(&e->data.items[e->cursor - 1],
			&e->data.items[e->cursor],
			e->data.count - e->cursor);
	e->cursor -= 1;
	e->data.count -= 1;
	editor_rebuild_lines(e);
}
