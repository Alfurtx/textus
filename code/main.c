#include "common.h"
#include "glyph.h"
#include "renderer.h"
#include "editor.h"

const uint WINDOW_W = 1920;
const uint WINDOW_H = 1080;

static GLFWwindow* window = NULL;
static char        current_dir[FILENAME_MAX];

static CharacterAtlas atlas = {0};
static Renderer renderer = {0};
static EditorState editor = {0};

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void resize_callback(GLFWwindow* win, int w, int h);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* window, uint codepoint);

int
main(void)
{
	/* GLFW Initializing */
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		window = glfwCreateWindow(WINDOW_W, WINDOW_H, "Textus", 0, 0);
		glfwMakeContextCurrent(window);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glfwSetFramebufferSizeCallback(window, resize_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetCharCallback(window, char_callback);
		glfwSwapInterval(1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(GLAD_GL_ARB_debug_output) {
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(message_callback, 0);
		} else {
			fprintf(stderr, "WARNING: GLAD_GL_ARB_debug_output is not available");
		}

	}


	/* FreeType Initializing */
    FT_Library ft;
    FT_Face face;
    char fontdir[FILENAME_MAX];
    get_current_dir(current_dir, FILENAME_MAX);
	{
		strcpy(fontdir, current_dir);
		strcat(fontdir, "/fonts/liberation.ttf");
		if(FT_Init_FreeType(&ft)) {
			fprintf(stderr, "Could not init freetype library\n");
			return 1;
		}
		if(FT_New_Face(ft, fontdir, 0, &face)) {
			fprintf(stderr, "Could not open font\n");
			return 1;
		}
		FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);
	}

	/* General Initialization */
	{
		char_atlas_init(&atlas, face);
		renderer_init(&renderer);
		editor_init(&editor, &atlas, window);
		editor_load_file(&editor, "/home/fonsi/proyectos/textus/test2.txt");
	}

	/* Render Loop */
	{
		while(!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(.2f, .5f, .9f, 1.0f);

			editor_render(&editor, &renderer);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	/* Cleanup */
	{
		editor_save_file(&editor);
		if(editor.data.items != NULL) free(editor.data.items);
		if(editor.filepath.items != NULL) free(editor.filepath.items);
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		glfwTerminate();
	}

    return 0;
}

void
message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;

    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type,
            severity,
            message);
}

void
resize_callback(GLFWwindow* win, int w, int h)
{
    glViewport(0, 0, w, h);
}

void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);

	// Movement
	if(editor.mode == NORMAL_MODE)
	{
		if(key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT ))
			editor_move_cursor_left(&editor);
		if(key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
			editor_move_cursor_right(&editor);
		if(key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
			editor_move_cursor_up(&editor);
		if(key == GLFW_KEY_J && (action == GLFW_PRESS || action == GLFW_REPEAT))
			editor_move_cursor_down(&editor);
	}

	// Mode Switching
	{
		if(editor.mode == INSERT_MODE) {
			if(key == GLFW_KEY_C && action == GLFW_PRESS &&
			   (mods & GLFW_MOD_CONTROL))
			{
				editor.mode = NORMAL_MODE;
				editor.mode_switch = true;
			}
		}

		if(editor.mode == NORMAL_MODE) {
			if(key == GLFW_KEY_I && action == GLFW_PRESS) {
				editor.mode = INSERT_MODE;
				editor.mode_switch = true;
			}
		}
	}

	if(key == GLFW_KEY_BACKSPACE &&
	   (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		editor_backspace(&editor);
	}

	if(key == GLFW_KEY_ENTER && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		if(editor.mode == INSERT_MODE) editor_insert_char(&editor, '\n');
		else if(editor.mode == NORMAL_MODE) editor_move_cursor_down(&editor);
	}
}

void
char_callback(GLFWwindow* window, uint codepoint)
{
	if(!editor.mode_switch) {
		if(editor.mode == INSERT_MODE)
			editor_insert_char(&editor, codepoint);
	}
	editor.mode_switch = false;
}
