#include "glyph.h"

void
character_atlas_init(CharacterAtlas* atlas, FT_Face face)
{
	for(uint i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed\n", i);
			exit(1);
		}

		atlas->atlas_width += face->glyph->bitmap.width;
		if (atlas->atlas_height < face->glyph->bitmap.rows)
            atlas->atlas_height = face->glyph->bitmap.rows;
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &atlas->texture_id);
	glBindTexture(GL_TEXTURE_2D, atlas->texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D,
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

		atlas->characters[i].ax = face->glyph->advance.x >> 6;
        atlas->characters[i].ay = face->glyph->advance.y >> 6;
        atlas->characters[i].bw = face->glyph->bitmap.width;
        atlas->characters[i].bh = face->glyph->bitmap.rows;
        atlas->characters[i].bl = face->glyph->bitmap_left;
        atlas->characters[i].bt = face->glyph->bitmap_top;
        atlas->characters[i].tx = (float) x / (float) atlas->atlas_width;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(GL_TEXTURE_2D,
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
character_atlas_render_line(CharacterAtlas* atlas,
							Renderer* r,
							const char* text,
							usize textsize,
							vec2* pos, vec4 color)
{
	for(usize i = 0; i < textsize; i++) {
		usize char_index = text[i];
		if(char_index >= CHARACTERINFO_CAP) {
			char_index = '?';
		}
		CharacterInfo m = atlas->characters[char_index];
		float x2 = pos->x + m.bl;
		float y2 = pos->y + m.bt;
		float w = m.bw;
		float h = m.bh;
		pos->x += m.ax;
		pos->y += m.ay;

		/*
		 * NOTE(fonsi): This will disable the little padding at the left of the text
		 * | Hola Mundo
		 * |Hola Mundo
		 */
		// x2 -= m.bl;

		renderer_push_image_rect(r,
								 Vec2Init(x2, y2),
								 Vec2Init(w, -h),
								 Vec2Init(m.tx, 0.0f),
								 Vec2Init(m.bw / (float) atlas->atlas_width,
										  m.bh / (float) atlas->atlas_height),
								 color);
	}
}
