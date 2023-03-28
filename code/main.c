#include "common.h"
#include "glyph.h"
#include "renderer.h"
#include "editor.h"

const uint WINDOW_W = 1920;
const uint WINDOW_H = 1080;

static GLFWwindow* window = NULL;
static char        current_dir[FILENAME_MAX];

static CharacterAtlas atlas    = {0};
static Renderer   renderer = {0};
static EditorState editor = {0};

Arena permanent_arena;

void MessageCallback(GLenum        source,
                     GLenum        type,
                     GLuint        id,
                     GLenum        severity,
                     GLsizei       length,
                     const GLchar* message,
                     const void*   userParam);
void ResizeCallback(GLFWwindow* win, int w, int h);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, ResizeCallback);
	glfwSetKeyCallback(window, key_callback);
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

    FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

    character_atlas_init(&atlas, face);
    renderer_init(&renderer);
	editor_init(&editor, &permanent_arena, &atlas);

	editor_load_file(&editor, "");

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.2f, .5f, .9f, 1.0f);

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

		editor_render(&editor, &renderer, window);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

	arena_release(&permanent_arena);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glfwTerminate();
    return 0;
}

void
MessageCallback(GLenum        source,
                GLenum        type,
                GLuint        id,
                GLenum        severity,
                GLsizei       length,
                const GLchar* message,
                const void*   userParam)
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
ResizeCallback(GLFWwindow* win, int w, int h)
{
    glViewport(0, 0, w, h);
}

void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT))
		editor_move_cursor_left(&editor);
	if(key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
		editor_move_cursor_right(&editor);
}
