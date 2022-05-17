#ifndef __T1__GUI__H__
#define __T1__GUI__H__

// No dynamic memory management occurs here.
// No internal copies are made.
// Redirects input from main module to menus and images modules.

#include "images.h"
#include "input.h"
#include "menus.h"
#include "vector.h"

struct gui
{
    enum input_state shift;
    // Input stuff.

    struct menus  menus;
    struct images images;
};

void gui_init  (struct gui *gui);
void gui_deinit(struct gui *gui);
void gui_key   (struct gui *gui, int keycode, enum input_state state);
void gui_mouse (struct gui *gui, union vector screen, struct input_mouse mouse);
void gui_render(struct gui *gui, union vector screen);

#endif // __T1__GUI__H__
