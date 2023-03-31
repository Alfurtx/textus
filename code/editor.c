#include "editor.h"

void
editor_init(EditorState* e, Arena* arena, CharacterAtlas* atlas, GLFWwindow* w)
{
	e->arena = arena;
	e->atlas = atlas;
	e->window = w;
}

usize
editor_get_cursor_row(EditorState* e)
{
	assert(e->lines.count > 0, "");
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
	// window info
	int w, h;
	glfwGetWindowSize(e->window, &w, &h);

	// update cursor info
	vec2 cursor_pos = editor_cursor_pos(e);
	usize cursor_width = e->atlas->characters['A'].ax;

	// render cursor
	{
		renderer_set_shader(renderer, SHADER_COLOR);
		renderer_push_rect(renderer, cursor_pos, Vec2Init(cursor_width, FONT_SIZE), Vec4Init(.0f, 1.0f, .0f, 1.0f));
		renderer_flush(renderer);
	}

	// render text
	{
		renderer_set_shader(renderer, SHADER_TEXT);
		usize lc = e->linebot - e->linetop + 1;
		for(usize i = 0; i < lc; i++) {
			usize lcount = e->lines.items[e->linetop + i].end -
				           e->lines.items[e->linetop + i].begin;
			vec2 tpos = Vec2Init(.0f, h - ((i + 1) * FONT_SIZE));
			Line lpos = e->lines.items[e->linetop + i];
			char_atlas_render_line(e->atlas,
								   renderer,
								   &e->data.items[lpos.begin],
								   lcount,
								   &tpos,
								   Vec4Inits(1.0f));
		}
		renderer_flush(renderer);
	}
}

void
editor_load_file(EditorState* e, const char* filepath)
{
	e->data.count = 0;
	read_file(filepath, &e->data);
	e->cursor = 0;
	e->scroll = 0;
	editor_rebuild_lines(e);

	e->filepath.count = 0;
	arr_append_cstr(&e->filepath, filepath);
	arr_append_null(&e->filepath);

	int w, h; glfwGetWindowSize(e->window, &w, &h);
	e->linetop = 0;
	e->linebot = h / FONT_SIZE - 1;

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
		if(e->data.items[e->cursor + 1] == '\n')
			e->cursor += 2;
		else
			e->cursor += 1;
	}
}

void
editor_move_cursor_left(EditorState* e)
{
	if(e->cursor > 0) {
		if(e->data.items[e->cursor - 1] == '\n')
			e->cursor -= 2;
		else
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

		if(crow - 1 < e->linetop) {
			e->scroll -= 1;
			if(e->linetop > 0) e->linetop--;
			if(e->linebot < e->lines.count) e->linebot--;
		}
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

		if(crow + 1 > e->linebot) {
			e->scroll += 1;
			if(e->linebot < e->lines.count - 1) {
				e->linebot++;
			}
			e->linetop++;
		}
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
		assert(e->data.items, "Realloc FAILED");
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
	assert(f, "Failed to detect file to write");
	fwrite(e->data.items, 1, e->data.count, f);
	fclose(f);
}

vec2
editor_cursor_pos(EditorState* e)
{
	int w, h; glfwGetWindowSize(e->window, &w, &h);
	vec2 result = {0};

	usize crow = editor_get_cursor_row(e);
	Line line = e->lines.items[crow];
	usize ccol = e->cursor - line.begin;
	result.y = h - (crow + 1 + CURSOR_OFFSET - e->scroll) * FONT_SIZE;
	result.x = char_atlas_cursor_pos(e->atlas,
									 e->data.items + line.begin,
									 line.end - line.begin,
									 ccol);
	return(result);
}
