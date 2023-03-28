#ifndef GLYPH_H
#define GLYPH_H

#include "common.h"
#include "renderer.h"

#define CHARACTERINFO_CAP 128
#define FONT_SIZE 48

typedef struct {
	float ax, ay; // advance.xy (vec2)
	float bw; // bitmap.width
	float bh; // bitmap.rows
	float bl; // bitmap_left
	float bt; // bitmap_top
	float tx; // x offset of glyph in texture coordinates
} CharacterInfo;

typedef struct GlyphAtlas {
	uint atlas_width;
	uint atlas_height;
	uint texture_id;
	CharacterInfo characters[CHARACTERINFO_CAP];
} CharacterAtlas;

void character_atlas_init(CharacterAtlas* atlas, FT_Face face);
void character_atlas_render_line(CharacterAtlas* atlas, Renderer* r, const char* text, usize textsize, vec2* pos, vec4 color);
vec2 get_cursor_pos_and_width(CharacterAtlas* atlas, const char* text, usize tsize, vec2 pos, usize col);

#endif
