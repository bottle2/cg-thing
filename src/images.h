#ifndef __T1__IMAGES__H__
#define __T1__IMAGES__H__

// Dynamic memory allocation occurs, internal copies happen.
// Controls all images: loading, caching, rendering, manipulation.

#include <stdbool.h>
#include <stdint.h>

#include "input.h"
#include "select.h"
#include "vector.h"

#define IMAGE_CAPACITY 256
#define N_TONE         256

enum channel { CHANNEL_RED, CHANNEL_GREEN, CHANNEL_BLUE };

struct images
{
    int   n_image;
    char *pathnames[IMAGE_CAPACITY];

    int widths [IMAGE_CAPACITY];
    int heights[IMAGE_CAPACITY];
    int xs     [IMAGE_CAPACITY];
    int ys     [IMAGE_CAPACITY];

    uint8_t *per_image_reds        [IMAGE_CAPACITY];
    uint8_t *per_image_greens      [IMAGE_CAPACITY];
    uint8_t *per_image_blues       [IMAGE_CAPACITY];
    float   *per_image_cache_reds  [IMAGE_CAPACITY];
    float   *per_image_cache_greens[IMAGE_CAPACITY];
    float   *per_image_cache_blues [IMAGE_CAPACITY];

    bool actives_red  [IMAGE_CAPACITY];
    bool actives_green[IMAGE_CAPACITY];
    bool actives_blue [IMAGE_CAPACITY];
    bool grayscales   [IMAGE_CAPACITY];
    bool inverteds    [IMAGE_CAPACITY];

    bool is_selecteds[IMAGE_CAPACITY];
    int  n_selected;

    struct select select;
    bool          is_moving;
};
// Images' data is associated by their position in the arrays.

void images_init   (struct images *images);
int  images_load   (struct images *images, char const *pathname, int x, int y);
void images_compute(struct images *images);
void images_free   (struct images *images);

int  images_mouse  (struct images *images, union vector screen, struct input_mouse mouse, enum input_state shift);
void images_render (struct images *images, union vector screen);

void images_toggle   (struct images *images, enum channel channel);
void images_reset    (struct images *images);
void images_grayscale(struct images *images);
void images_invert   (struct images *images);

#endif // __T1__IMAGES__H__
