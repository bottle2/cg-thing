#ifndef __T1__MENUS__H__
#define __T1__MENUS__H__

// Manages menus. Creation, rendering, mouse input and callback calling.
// NOTE Module refactored out of gui module, may contain "rabos" left over.

#define FONT_MONOSPACE_WIDTH  8
#define FONT_MONOSPACE_HEIGHT 13
#define FONT_WIDTH            FONT_MONOSPACE_WIDTH
#define FONT_HEIGHT           FONT_MONOSPACE_HEIGHT
#define FONT_KERNING          2

#define ITEM_CAPACITY 50
#define DATA_CAPACITY  5

#define ITEM_ID_UNNOCUPIED 0

enum item_type { ITEM_NONE, ITEM_TEXT, ITEM_BUTTON };

struct menus
{
    int absolute_item_i_chosen;

    void *data[DATA_CAPACITY];
    // Data for callbacks.

    int              menu_active_id;
    int              max_item;
    enum item_type   items_type      [ITEM_CAPACITY];
    int              items_menu_id   [ITEM_CAPACITY];
    char const      *items_text      [ITEM_CAPACITY];
    char const      *items_text_hover[ITEM_CAPACITY];
    void           (*items_callback  [ITEM_CAPACITY])(void *);
    int              items_data_id   [ITEM_CAPACITY];
    // Lists are kept in a contiguous array, each differed in `items_list_id`.
    // A 0 in `items_list_id` means the position can store another item.
    // List items are not necessarily consecutive, but are always in order.
    // List items' data is associated across arrays through its position.
    // `max_item` tracks the biggest item position used.
    // It is not tracked how many items are there, let alone for each list.
    // TODO Update long comment.

    int width;
};

void menus_init(struct menus *menus);

void menus_set_data(struct menus *menus, int id, void *data);

void menus_add_text(struct menus *menus, int menu_id, char const *text);

void menus_add_button(
    struct menus *menus,
    int           menu_id,
    char const   *text,
    char const   *text_hover,
    void        (*callback)(void *),
    int           data_id
);

void menus_clear(struct menus *menus, int menu_id);
// List creation functions.
// Notice you can only add items or clear an entire list.

void menus_render(struct menus *menus, int screen_height);
int  menus_trace(struct menus *menus, int screen_height, int x, int y);
int  menus_click(struct menus *menus);

#endif // __T1__MENUS__H__
