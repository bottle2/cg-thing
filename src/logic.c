#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "images.h"
#include "logic.h"
#include "menus.h"

#define MAX_FILES_PER_TIME 15

#define PREFIX "bento-b-schirmer-trabalho-1-cg-2022/"

// TODO Investigate simplification with X Macros.
// TODO Implement push and pop menus in menus module.

enum
{
    MAIN  = 1,
    IMAGE = 2,
    ALL   = MAIN | IMAGE,
    HELP  = 4,
};

enum
{
    DATA_NULL,
    DATA_GUI,
    DATA_MENUS,
    DATA_IMAGES,
};

static void callback_placeholder(void *data)
{
    (void)data;
    // Do nothing.
}

static void callback_to_help(void *data)
{
    assert(data != NULL);
    ((struct menus *)data)->menu_active_id = HELP;
}

static void callback_to_main(void *data)
{
    assert(data != NULL);
    ((struct menus *)data)->menu_active_id = MAIN;
}

static void callback_quit(void *data)
{
    (void)data;
    exit(EXIT_SUCCESS);
}

static void callback_toggle_red(void *data)
{
    assert(data != NULL);

    images_toggle((struct images *)data, CHANNEL_RED);
}

static void callback_toggle_green(void *data)
{
    assert(data != NULL);

    images_toggle((struct images *)data, CHANNEL_GREEN);
}

static void callback_toggle_blue(void *data)
{
    assert(data != NULL);

    images_toggle((struct images *)data, CHANNEL_BLUE);
}

static void callback_reset(void *data)
{
    assert(data != NULL);

    images_reset((struct images *)data);
}

static void callback_grayscale(void *data)
{
    assert(data != NULL);

    images_grayscale((struct images *)data);
}

static void callback_invert(void *data)
{
    assert(data != NULL);

    images_invert((struct images *)data);
}

static void callback_list_files(void *data)
{
    struct gui    *gui    = (struct gui *)data;
    struct images *images = &(gui->images);

    int x = 400;
    int y = 400;

    #if 0
    images_load(images, "a.bmp");
    images_load(images, "all_gray.bmp");
    images_load(images, "b.bmp");
    images_load(images, "bmp_08.bmp");
    images_load(images, "c.bmp");
    images_load(images, "dots.bmp");
    images_load(images, "greenland_grid_velo.bmp");
    images_load(images, "lena.bmp");
    #endif
    images_load(images, PREFIX "resources/bmp_24.bmp"   , x, y);
    images_load(images, PREFIX "resources/snail.bmp"    , x, y);
    images_load(images, PREFIX "resources/blackbuck.bmp", x, y);
}

void logic_init(struct gui *gui)
{
    struct menus *menus = &(gui->menus);

    menus_set_data(menus, DATA_NULL  , NULL);
    menus_set_data(menus, DATA_GUI   , gui);
    menus_set_data(menus, DATA_MENUS , menus);
    menus_set_data(menus, DATA_IMAGES, &(gui->images));

    menus_add_button(menus, ALL, "  Help"       , "> Help"       , callback_to_help    , DATA_MENUS);
    menus_add_button(menus, ALL, "  Load images", "> Load images", callback_list_files , DATA_GUI);
    menus_add_button(menus, ALL, "  Quit"       , "> Quit"       , callback_quit       , 0);

    //menus_add_button(menus, IMAGE, "  Delete"                 , "> Delete"                 , callback_placeholder, 0);
    //menus_add_button(menus, IMAGE, "  Flip horizontally"      , "> Flip horizontally"      , callback_placeholder, 0);
    //menus_add_button(menus, IMAGE, "  Flip vertically"        , "> Flip vertically"        , callback_placeholder, 0);
    //menus_add_button(menus, IMAGE, "  Toggle histogram"       , "> Toggle histogram"       , callback_histogram   , DATA_IMAGES);
    //menus_add_button(menus, IMAGE, "  Halve size"             , "> Halve size"             , callback_placeholder, 0);
    //menus_add_button(menus, IMAGE, "  Double size"            , "> Double size"            , callback_placeholder, 0);
    menus_add_button(menus, IMAGE, "  Render normally"        , "> Render normally"        , callback_reset       , DATA_IMAGES);
    menus_add_button(menus, IMAGE, "  Grayscale render"       , "> Grayscale render"       , callback_grayscale   , DATA_IMAGES);
    menus_add_button(menus, IMAGE, "  Toggle red channel"     , "> Toggle red channel"     , callback_toggle_red  , DATA_IMAGES);
    menus_add_button(menus, IMAGE, "  Toggle green channel"   , "> Toggle green channel"   , callback_toggle_green, DATA_IMAGES);
    menus_add_button(menus, IMAGE, "  Toggle blue channel"    , "> Toggle blue channel"    , callback_toggle_blue , DATA_IMAGES);
    menus_add_button(menus, IMAGE, "  Toggle inverted"        , "> Toggle inverted"        , callback_invert      , DATA_IMAGES);
    //menus_add_button(menus, IMAGE, "  Rotate clockwise"       , "> Rotate clockwise"       , callback_placeholder, 0);
    //menus_add_button(menus, IMAGE, "  Rotate counterclockwise", "> Rotate counterclockwise", callback_placeholder, 0);
    //menus_add_button(menus, IMAGE, "  Brighten"               , "> Brighten"               , callback_placeholder, 0);
    //menus_add_button(menus, IMAGE, "  Darken"                 , "> Darken"                 , callback_placeholder, 0);

    menus_add_button(menus, HELP, "  Go back", "< Go back", callback_to_main, DATA_MENUS);
    menus_add_text  (menus, HELP, "Click on images to select them.");
    menus_add_text  (menus, HELP, "Hold Shift to select multiple images.");
    menus_add_text  (menus, HELP, "Images are loaded on top of each other.");
    menus_add_text  (menus, HELP, "Move images apart to show the hidden ones.");
    menus_add_text  (menus, HELP, "Images start selected, click outside to deselect.");
    menus_add_text  (menus, HELP, "Press Q or Esc to exit.");
    menus_add_text  (menus, HELP, "Histograms are in logarithm scale.");

    menus->menu_active_id = MAIN;
}

void logic_kludge(struct gui *gui)
{
	if (gui->menus.menu_active_id & MAIN)
	{
		gui->menus.menu_active_id = gui->images.n_selected > 0 ? ALL : MAIN;
	}
}

