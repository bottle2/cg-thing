#include <assert.h>
#include <stddef.h>
#include <stdio.h> // XXX Remover
#include <string.h>

#include "gl_canvas2d.h"

#include "menus.h"

#define FOREGROUND 1.0f, 1.0f, 1.0f

static int menus_next_unnocupied(struct menus *menus); // Checks for bounds.

void menus_init(struct menus *menus)
{
    menus->absolute_item_i_chosen = -1;

    menus->max_item = 0;

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
    menus->items_width[item_i]      = -1;
    // Paranoia.
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
    assert(strlen(text) == strlen(text_hover));
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
    menus->items_width[item_i]      = strlen(text) * (FONT_WIDTH + FONT_KERNING);
}

void menus_clear(struct menus *menus, int menu_id)
{
    assert(menus != NULL);
    assert(menu_id != 0);

    for (int item_i = menus->max_item; item_i >= 0; item_i--)
    {
        if (menus->items_menu_id[item_i] & menu_id)
        {
            menus->items_type[item_i]       = ITEM_NONE;
            menus->items_menu_id[item_i]    = ITEM_ID_UNNOCUPIED;
            menus->items_text[item_i]       = NULL;
            menus->items_text_hover[item_i] = NULL;
            menus->items_callback[item_i]   = NULL;
            menus->items_data_id[item_i]    = -1;
            menus->items_width[item_i]      = -1;

            if (item_i == menus->max_item - 1)
            {
                menus->max_item = item_i;
            }
            // Decrease `max_item` if removing the last item.
        }
    }
}

void menus_render(struct menus *menus, int screen_height)
{
    assert(menus->menu_active_id != 0);

    cv_color_rgb(FOREGROUND);

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

        cv_text(0, (float)(screen_height - (actual_item_i + 1) * FONT_HEIGHT), text);

        actual_item_i++;
    }
}

int menus_trace(struct menus *menus, int screen_height, int x, int y)
{
    int relative_y = (screen_height - y) / FONT_HEIGHT;

    menus->absolute_item_i_chosen = -1;

    for (int item_i = 0, relative_i = 0; item_i <= menus->max_item; item_i++)
    {
        if (menus->items_menu_id[item_i] & menus->menu_active_id)
        {
            if(x < menus->items_width[item_i] && relative_y == relative_i)
            {
                menus->absolute_item_i_chosen = item_i;
                return item_i;
            }

            relative_i++;
        }
    }

    return -1;
}

int menus_click(struct menus *menus)
{
    if (ITEM_BUTTON == menus->items_type[menus->absolute_item_i_chosen])
    {
        int data_id = menus->items_data_id[menus->absolute_item_i_chosen];
        menus->items_callback[menus->absolute_item_i_chosen](menus->data[data_id]);
        return 0;
    }
    return 1;
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
