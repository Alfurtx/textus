#ifndef GLYPH_H
#define GLYPH_H

#include "common.h"

#define GLYPH_INFO_CAPACITY 128

typedef struct {
	float ax, ay;
	float bw, bh;
	float bl, bt;
	float tx;
} GlyphInfo;

typedef struct GlyphAtlas {
	FT_UInt atlas_width;
	FT_UInt atlas_height;
	GLuint glyphs_texture;
	GlyphInfo glyphs[GLYPH_INFO_CAPACITY];
} GlyphAtlas;

void GlyphAtlasInit(GlyphAtlas* atlas, FT_Face face);

#endif
