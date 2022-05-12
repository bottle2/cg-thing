// Canvas2D's callbacks are configured here.
// The gui module handles all events and rendering.
// Check README.txt for implemented features.
// Instructions inside program.

#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "gl_canvas2d.h"

#include "gui.h"
#include "input.h"
#include "logic.h"

#define BACKGROUND 0.0f, 0.0f, 0.0f

int screen_width = 800, screen_height = 600;
static struct gui gui;

void render(void)
{
    CV::clear(BACKGROUND);

    gui_dimensions(&gui, screen_width, screen_height);
    gui_render(&gui);
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
    gui_dimensions(&gui, screen_width, screen_height);
    gui_mouse(&gui, x, y, (enum input_mouse)button, (enum input_state)state);

    if (INPUT_SCROLL_YES == wheel)
    {
        gui_scroll(&gui, (enum input_direction)direction);
    }

    if (INPUT_PRESSED == state && INPUT_MOUSE_LEFT == button)
    {
        logic_kludge(&gui);
    }
}

int main(void)
{
    gui_init(&gui);

    logic(&gui);

    CV::init(&screen_width, &screen_height, "Manipulação de imagens");

    CV::run();

    gui_deinit(&gui);

    return 0;
}
