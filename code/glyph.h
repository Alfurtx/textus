#ifndef GLYPH_H
#define GLYPH_H

#include "common.h"
#include "renderer.h"

#define CHARACTERINFO_CAP 128

typedef struct {
	float ax, ay;
	float bw, bh;
	float bl, bt;
	float tx;
} CharacterInfo;

typedef struct GlyphAtlas {
	uint atlas_width;
	uint atlas_height;
	uint texture_id;
	CharacterInfo characters[CHARACTERINFO_CAP];
} CharacterAtlas;

void character_atlas_init(CharacterAtlas* atlas, FT_Face face);
void character_atlas_render_line(CharacterAtlas* atlas, Renderer* r, const char* text, usize textsize, vec2* pos, vec4 color);

#endif
