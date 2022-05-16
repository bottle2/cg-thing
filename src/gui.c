#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "images.h"
#include "input.h"
#include "menus.h"

#define IMAGES_INITIAL_CAPACITY 2

void gui_init(struct gui *gui)
{
    assert(gui != NULL);

    gui->shift = INPUT_RELEASED;

    menus_init(&(gui->menus));

    images_init(&(gui->images));
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

void gui_mouse(
    struct gui       *gui,
    int               x,
    int               y,
    enum input_mouse  button,
    enum input_state  state
) {
    if (-1 == images_trace(&(gui->images), x, y))
    {
        menus_trace(&(gui->menus), gui->screen_height, x, y);
    }
    else
    {
        gui->menus.absolute_item_i_chosen = -1;
    }

    if (INPUT_PRESSED == state && INPUT_MOUSE_LEFT == button)
    {
        if (!menus_click(&(gui->menus)))
        {
            return;
        }
    }

    images_click(&(gui->images), x, y, state, INPUT_PRESSED == gui->shift);
}

void gui_scroll(struct gui *gui, enum input_direction direction)
{
    // TODO Handle scroll for file list in loader, resize and sprite order.

    (void)gui;
    (void)direction;
}

void gui_dimensions(struct gui *gui, int screen_width, int screen_height)
{
    gui->screen_width  = screen_width;
    gui->screen_height = screen_height;
}

void gui_render(struct gui *gui)
{
    menus_render(&(gui->menus), gui->screen_height);
    images_render(&(gui->images), gui->screen_width, gui->screen_height);
}
