#include "glyph.h"

void
GlyphAtlasInit(GlyphAtlas* atlas, FT_Face face)
{
	for(uint i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed\n", i);
			exit(1);
		}

		atlas->atlas_width += face->glyph->bitmap.width;
		if (atlas->atlas_height < face->glyph->bitmap.rows) {
            atlas->atlas_height = face->glyph->bitmap.rows;
        }
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &atlas->glyphs_texture);
	glBindTexture(GL_TEXTURE_2D, atlas->glyphs_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
				 GL_TEXTURE_2D,
				 0,
				 GL_RED,
				 (GLsizei) atlas->atlas_width,
				 (GLsizei) atlas->atlas_height,
				 0,
				 GL_RED,
				 GL_UNSIGNED_BYTE,
				 NULL);

	int x = 0;
	for(uint i = 32; i < 128; ++i) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr,
					"ERROR: could not load glyph of a character with code %d\n",
					i);
            exit(1);
        }

        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) {
            fprintf(stderr,
					"ERROR: could not render glyph of a character with code %d\n",
					i);
            exit(1);
        }

		atlas->glyphs[i].ax = face->glyph->advance.x >> 6;
        atlas->glyphs[i].ay = face->glyph->advance.y >> 6;
        atlas->glyphs[i].bw = face->glyph->bitmap.width;
        atlas->glyphs[i].bh = face->glyph->bitmap.rows;
        atlas->glyphs[i].bl = face->glyph->bitmap_left;
        atlas->glyphs[i].bt = face->glyph->bitmap_top;
        atlas->glyphs[i].tx = (float) x / (float) atlas->atlas_width;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(
						GL_TEXTURE_2D,
						0,
						x,
						0,
						face->glyph->bitmap.width,
						face->glyph->bitmap.rows,
						GL_RED,
						GL_UNSIGNED_BYTE,
						face->glyph->bitmap.buffer);
        x += face->glyph->bitmap.width;
	}
}

void
GlyphAtlasRenderLine(GlyphAtlas* atlas,
					 Renderer* r,
					 const char* text,
					 usize textsize,
					 vec2* pos, vec4 color)
{
	for(usize i = 0; i < textsize; i++) {
		usize gi = text[i];
		if(gi >= GLYPH_INFO_CAPACITY) {
			gi = '?';
		}
		GlyphInfo m = atlas->glyphs[gi];
		float x2 = pos->x + m.bl;
		float y2 = -pos->y + m.bt;
		float w = m.bw;
		float h = m.bh;

		pos->x += m.ax;
		pos->y += m.ay;

		RendererImageRect(r,
						  Vec2Init(x2, -y2),
						  Vec2Init(w, -h),
						  Vec2Init(m.tx, 0.0f),
						  Vec2Init(m.bw / (float) atlas->atlas_width,
								   m.bh / (float) atlas->atlas_height),
						  color);
	}
}
