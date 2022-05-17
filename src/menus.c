#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "gl_canvas2d.h"

#include "menus.h"
#include "util.h"

static int menus_next_unnocupied(struct menus *menus); // Checks for bounds.

void menus_init(struct menus *menus)
{
    menus->absolute_item_i_chosen = -1;
    menus->max_item               =  0;
    menus->width                  =  0;

    for (int item_i = 0; item_i < ITEM_CAPACITY; item_i++)
    {
        menus->items_menu_id[item_i] = 0;
    }
}

void menus_set_data(struct menus *menus, int id, void *data)
{
    assert(menus != NULL);
    assert(id >= 0);
    assert(id < DATA_CAPACITY);

    menus->data[id] = data;
}

void menus_add_text(struct menus *menus, int menu_id, char const *text)
{
    assert(menus != NULL);
    assert(menu_id != ITEM_ID_UNNOCUPIED);
    assert(text != NULL);

    int item_i = menus_next_unnocupied(menus);

    menus->items_type[item_i]    = ITEM_TEXT;
    menus->items_menu_id[item_i] = menu_id;
    menus->items_text[item_i]    = text;

    menus->items_text_hover[item_i] = NULL;
    menus->items_callback[item_i]   = NULL;
    menus->items_data_id[item_i]    = -1;
    // Paranoia.

    menus->width = MAX_2(menus->width, strlen(text));
}


void menus_add_button(
    struct menus *menus,
    int           menu_id,
    char const   *text,
    char const   *text_hover,
    void        (*callback)(void *),
    int           data_id
) {
    assert(menus != NULL);
    assert(menu_id != ITEM_ID_UNNOCUPIED);
    assert(text       != NULL);
    assert(text_hover != NULL);
    assert(callback != NULL);
    assert(data_id >= 0);
    assert(data_id <  DATA_CAPACITY);

    int item_i = menus_next_unnocupied(menus);

    menus->items_type[item_i]       = ITEM_BUTTON;
    menus->items_menu_id[item_i]    = menu_id;
    menus->items_text[item_i]       = text;
    menus->items_text_hover[item_i] = text_hover;
    menus->items_callback[item_i]   = callback;
    menus->items_data_id[item_i]    = data_id;

    menus->width = MAX_2(menus->width, strlen(text      ));
    menus->width = MAX_2(menus->width, strlen(text_hover));
}

int menus_mouse(struct menus *menus, union vector screen, struct input_mouse mouse)
{
    assert(menus != NULL);

    int relative_y = (screen.height - mouse.y) / FONT_HEIGHT;

    menus->absolute_item_i_chosen = -1;

    for (int item_i = 0, relative_i = 0; item_i <= menus->max_item; item_i++)
    {
        if (menus->items_menu_id[item_i] & menus->menu_active_id)
        {
            if (mouse.x < menus->width * (FONT_WIDTH + FONT_KERNING)
                && relative_y == relative_i
                && ITEM_BUTTON == menus->items_type[item_i]
            ) {

                if (INPUT_PRESSED == mouse.state
                    && INPUT_MOUSE_LEFT == mouse.button
                    && ITEM_BUTTON == menus->items_type[item_i]
                ) {
                    int data_id = menus->items_data_id[item_i];
                    menus->items_callback[item_i](menus->data[data_id]);
                }
                else
                {
                    menus->absolute_item_i_chosen = item_i;
                }
                // Handles clicks.

                return 1;
            }
            // Checks for mouse hover. 

            relative_i++;
        }
        // We iterate only over items from active menu.
    }

    return 0;
}

void menus_render(struct menus *menus, union vector screen)
{
    assert(menus->menu_active_id != 0);

    cv_color_rgb(1.0f, 1.0f, 1.0f);

    int actual_item_i = 0;

    for (int item_i = 0; item_i <= menus->max_item; item_i++)
    {
        if (!(menus->items_menu_id[item_i] & menus->menu_active_id))
        {
            continue;
        }

        char const *text = menus->absolute_item_i_chosen == item_i
                         ? menus->items_text_hover[item_i]
                         : menus->items_text[item_i];

        cv_text(0, (float)(screen.height - (actual_item_i + 1) * FONT_HEIGHT), text);

        actual_item_i++;
    }
}

static int menus_next_unnocupied(struct menus *menus)
{
    for (int item_i = 0; item_i < ITEM_CAPACITY; item_i++)
    {
        if (0 == menus->items_menu_id[item_i])
        {
            assert(item_i <= menus->max_item + 1);

            if (item_i > menus->max_item)
            {
                menus->max_item = item_i;
            }

            return item_i;
        }
    }

    assert(!"No space for more items. Increase ITEM_CAPACITY.");

    return -1;
}
