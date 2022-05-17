#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "images.h"
#include "input.h"
#include "logic.h"
#include "menus.h"
#include "vector.h"

void gui_init(struct gui *gui)
{
    assert(gui != NULL);

    gui->shift = INPUT_RELEASED;

    menus_init (&(gui->menus));
    images_init(&(gui->images));
    logic_init(gui);
}

void gui_deinit(struct gui *gui)
{
    images_free(&(gui->images));
}

void gui_key(struct gui *gui, int keycode, enum input_state state)
{
    if (INPUT_PRESSED == state && (INPUT_KEY_ESC == keycode || 'q' == keycode))
    {
        exit(EXIT_SUCCESS);
    }

    if (INPUT_KEY_SHIFT == keycode)
    {
        gui->shift = state;
    }
}

void gui_mouse(struct gui *gui, union vector screen, struct input_mouse mouse)
{
    assert(gui != NULL);

    if (menus_mouse (&(gui->menus ), screen, mouse            )) return;
    if (images_mouse(&(gui->images), screen, mouse, gui->shift)) return;
}

void gui_render(struct gui *gui, union vector screen)
{
    images_render(&(gui->images), screen);
    menus_render (&(gui->menus ), screen);
}
