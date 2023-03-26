#include "common.h"
#include "glyph.h"
#include "renderer.h"

typedef struct {
	GlyphAtlas atlas;
	FT_Face face;
} EditorState;

extern EditorState editor_state;
