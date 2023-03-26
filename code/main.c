#include "common.h"
#include "renderer.h"
#include "textus.h"

const uint WINDOW_W = 1920;
const uint WINDOW_H = 1080;

EditorState editor_state;

static GLFWwindow* window = NULL;
static char current_dir[FILENAME_MAX];

int
main(void)
{
    GetCurrentDir(current_dir, FILENAME_MAX);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Textus", 0, 0);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

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
	if(FT_New_Face(ft, fontdir, 0, &editor_state.face)) {
		fprintf(stderr, "Could not open font\n");
		return 1;
	}

	FT_Set_Pixel_Sizes(editor_state.face, 0, 64);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(.2f, .5f, .5f, 1.0f);

		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, 1);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	FT_Done_Face(editor_state.face);
	FT_Done_FreeType(ft);
	glfwTerminate();
	return 0;
}
