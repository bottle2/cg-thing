#ifndef __T1__GUI__H__
#define __T1__GUI__H__

// No dynamic memory management occurs here.
// No internal copies are made.
// Redirects input from main module to menus and images modules.

#include "images.h"
#include "input.h"
#include "menus.h"

struct gui
{
    enum input_state shift;
    // Input stuff.

    struct menus menus;

    int screen_width;
    int screen_height;
    // Extents data. All measured in pixels.

    struct images images;
};

void gui_init  (struct gui *gui);
void gui_deinit(struct gui *gui);

void gui_key        (struct gui *gui, int keycode, enum input_state state);
void gui_mouse(struct gui *gui, int x, int y
                              , enum input_mouse button
                              , enum input_state state);
void gui_scroll     (struct gui *gui, enum input_direction direction);
void gui_dimensions (struct gui *gui, int screen_width, int screen_height);
// Event handling.
// `gui_mouse_click()` and `gui_scroll()` checks if click or scroll happened.

void gui_render(struct gui *gui);
// Render!

#endif // __T1__GUI__H__
