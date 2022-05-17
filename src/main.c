// Canvas2D's callbacks are configured here.
// The gui module handles all events and rendering.
// Check README.txt for implemented features.
// Instructions inside program.

#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "gl_canvas2d.h"

#include "gui.h"
#include "input.h"
#include "vector.h"

static union  vector screen = { .width = 800, .height = 800 };
static struct gui    gui    = { 0 };

void render(void)
{
    cv_clear(0.0f, 0.0f, 0.0f);

    gui_render(&gui, screen);
}

void keyboard(int key)
{
    gui_key(&gui, key, INPUT_PRESSED);
}

void keyboardUp(int key)
{
    gui_key(&gui, key, INPUT_RELEASED);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    struct input_mouse mouse = { x, y, button, state, wheel, direction };
    gui_mouse(&gui, screen, mouse);
}

int main(void)
{
    gui_init(&gui);
    cv_init(&(screen.width), &(screen.height), "Manipulação de imagens");
    cv_run();
    gui_deinit(&gui);

    return 0;
}
