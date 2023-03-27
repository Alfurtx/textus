#include "common.h"
#include "glyph.h"
#include "renderer.h"

const uint WINDOW_W = 1920;
const uint WINDOW_H = 1080;

static GLFWwindow* window = NULL;
static char current_dir[FILENAME_MAX];

static GlyphAtlas atlas = {0};
static Renderer renderer = {0};

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void ResizeCallback(GLFWwindow* win, int w, int h);

int
main(void)
{
    GetCurrentDir(current_dir, FILENAME_MAX);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Textus", 0, 0);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSetFramebufferSizeCallback(window, ResizeCallback);
	glfwSwapInterval(1);

	if(GLAD_GL_ARB_debug_output) {
		glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    } else {
        fprintf(stderr, "WARNING: GLEW_ARB_debug_output is not available");
    }

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// FONT Loading
	char fontdir[FILENAME_MAX];
	strcpy(fontdir, current_dir);
	strcat(fontdir, "/fonts/iosevka.ttf");
	FT_Library ft;
	if(FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return 1;
	}
	FT_Face face;
	if(FT_New_Face(ft, fontdir, 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return 1;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	GlyphAtlasInit(&atlas, face);
	RendererInit(&renderer);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT );
		glClearColor(.2f, .5f, .9f, 1.0f);

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, 1);

		renderer.resolution = Vec2Init(1920, 1080);
		renderer.time = glfwGetTime();

		vec2 pos = Vec2Init(100.0f, 100.0f);
		RendererSetShader(&renderer, SHADER_TEXT);
		GlyphAtlasRenderLine(&atlas, &renderer, "HOLA MUNDO", 10 * sizeof(char), &pos, Vec4Init(1.0f, 1.0f, 1.0f, 1.0f));
		// RendererSetShader(&renderer, SHADER_COLOR);
		// RendererRect(&renderer, Vec2Init(0.0f, 0.0f), Vec2Init(1920.0f, 1080.0f), Vec4Init(1.0f, 0.0f, 0.0f, 1.0f));

		RendererFlush(&renderer);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	glfwTerminate();
	return 0;
}

void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam)
{
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

void
ResizeCallback(GLFWwindow* win, int w, int h)
{
	glViewport(0,0,w,h);
}
