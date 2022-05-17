#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "gl_canvas2d.h"

#include "bmp.h"
#include "framebuffer.h"
#include "images.h"
#include "input.h"
#include "safe.h"
#include "select.h"
#include "sources.h"

void images_init(struct images *images)
{
    assert(images != NULL);

    images->n_image    = 0;
    images->n_selected = 0;
    images->is_moving  = false;

    select_init (&(images->select ));
    sources_init(&(images->sources));
}

int images_load(struct images *images, char const *pathname, int x, int y)
{
    assert(images   != NULL);
    assert(pathname != NULL);
    assert(images->n_image < IMAGE_CAPACITY);

    int source_i = sources_load(&(images->sources), pathname);

    if (-1 == source_i)
    {
        return -1;
    }

    int image_n = images->n_image;

    int n_pixel = images->sources.widths[source_i] * images->sources.heights[source_i];

    float *cache_reds   = (float *)safe_malloc(n_pixel, sizeof (float));
    float *cache_greens = (float *)safe_malloc(n_pixel, sizeof (float));
    float *cache_blues  = (float *)safe_malloc(n_pixel, sizeof (float));

    images->n_image++;

    images->per_image_source_i    [image_n] = source_i;
    images->per_image_cache_reds  [image_n] = cache_reds;
    images->per_image_cache_greens[image_n] = cache_greens;
    images->per_image_cache_blues [image_n] = cache_blues;

    images->actives_red  [image_n] = true;
    images->actives_green[image_n] = true;
    images->actives_blue [image_n] = true;

    images->xs[image_n] = x;
    images->ys[image_n] = y;

    images->n_selected++;
    images->is_selecteds[image_n] = true;

    images_compute(images);

    return 0;
}

void images_compute(struct images *images)
{
    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        if (images->is_selecteds[image_i])
        {
            int source_i = images->per_image_source_i[image_i];
            int width    = images->sources.widths [source_i];
            int height   = images->sources.heights[source_i];
            int n_pixel  = width * height;

            uint8_t *reds   = images->sources.per_image_reds  [source_i];
            uint8_t *greens = images->sources.per_image_greens[source_i];
            uint8_t *blues  = images->sources.per_image_blues [source_i];

            float *cache_reds   = images->per_image_cache_reds  [image_i];
            float *cache_greens = images->per_image_cache_greens[image_i];
            float *cache_blues  = images->per_image_cache_blues [image_i];

            bool active_red   = images->actives_red  [image_i];
            bool active_green = images->actives_green[image_i];
            bool active_blue  = images->actives_blue [image_i];
            bool is_grayscale = images->grayscales   [image_i];
            bool is_inverted  = images->inverteds    [image_i];

            assert(width  > 0);
            assert(height > 0);
            assert(reds   != NULL);
            assert(greens != NULL);
            assert(blues  != NULL);
            assert(cache_reds   != NULL);
            assert(cache_greens != NULL);
            assert(cache_blues  != NULL);

            for (int pixel_i = 0; pixel_i < n_pixel; pixel_i++)
            {
                cache_reds  [pixel_i] = active_red   ? (float)reds[pixel_i]   / 255.0f : 0.0f;
                cache_greens[pixel_i] = active_green ? (float)greens[pixel_i] / 255.0f : 0.0f;
                cache_blues [pixel_i] = active_blue  ? (float)blues[pixel_i]  / 255.0f : 0.0f;

                if (is_grayscale)
                {
                    float luminancy = cache_reds[pixel_i]   * 0.299f
                                    + cache_greens[pixel_i] * 0.587f
                                    + cache_blues[pixel_i]  * 0.144f;

                    cache_reds[pixel_i]   = luminancy;
                    cache_greens[pixel_i] = luminancy;
                    cache_blues[pixel_i]  = luminancy;
                }

                if (is_inverted)
                {
                    cache_reds[pixel_i]   = 1.0f - cache_reds[pixel_i];
                    cache_greens[pixel_i] = 1.0f - cache_greens[pixel_i];
                    cache_blues[pixel_i]  = 1.0f - cache_blues[pixel_i];
                }
            }
        }
    }
}

void images_render(struct images *images, union vector screen)
{
    assert(images != NULL);
    assert(images->n_image >= 0);

    int offset_x = images->is_moving ? images->select.offset_x : 0;
    int offset_y = images->is_moving ? images->select.offset_y : 0;

    struct framebuffer framebuffer = { 0 };

    framebuffer_init(&framebuffer, screen.width, screen.height);

    for (int image_i = images->n_image - 1; image_i >= 0; image_i--)
    {
        float *reds   = images->per_image_cache_reds[image_i];
        float *greens = images->per_image_cache_greens[image_i];
        float *blues  = images->per_image_cache_blues[image_i];

        int    source_i    = images->per_image_source_i[image_i];
        int    width       = images->sources.widths [source_i];
        int    height      = images->sources.heights[source_i];
        int    x           = images->xs[image_i] - width  / 2;
        int    y           = images->ys[image_i] - height / 2;
        bool   is_selected = images->is_selecteds[image_i];

        assert(reds   != NULL);
        assert(greens != NULL);
        assert(blues  != NULL);
        assert(width  > 0);
        assert(height > 0);

        framebuffer_burn(
            &framebuffer,
            width,
            height,
            x + (is_selected ? offset_x : 0),
            y + (is_selected ? offset_y : 0),
            reds,
            greens,
            blues
        );
    }

    framebuffer_render(&framebuffer);
    framebuffer_free(&framebuffer);

    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        int    source_i    = images->per_image_source_i[image_i];
        int    width       = images->sources.widths [source_i];
        int    height      = images->sources.heights[source_i];
        int    x           = images->xs[image_i] - width  / 2;
        int    y           = images->ys[image_i] - height / 2;
        bool   is_selected = images->is_selecteds[image_i];

        assert(width  > 0);
        assert(height > 0);

        if (images->is_selecteds[image_i])
        {
            cv_color_rgb(1.0f, 1.0f, 0.0f);
            cv_rect(
                (float)(x - 5          + (is_selected ? offset_x : 0)),
                (float)(y - 5          + (is_selected ? offset_y : 0)),
                (float)(x + width  + 5 + (is_selected ? offset_x : 0)),
                (float)(y + height + 5 + (is_selected ? offset_y : 0))
            );
        }
    }
}

void images_free(struct images *images)
{
    assert(images != NULL);

    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        free(images->pathnames[image_i]);

        free(images->per_image_cache_reds  [image_i]);
        free(images->per_image_cache_greens[image_i]);
        free(images->per_image_cache_blues [image_i]);
    }

    images->n_image = 0;

    sources_free(&(images->sources));
}

int images_mouse(struct images *images, union vector screen, struct input_mouse mouse, enum input_state shift)
{
    int hovered_i = -1;

    for (int image_i = images->n_image - 1; image_i >= 0; image_i--)
    {
        int source_i = images->per_image_source_i[image_i];
        int width    = images->sources.widths [source_i];
        int height   = images->sources.heights[source_i];
        int image_x  = images->xs[image_i];
        int image_y  = images->ys[image_i];

        bool within_x = mouse.x <= image_x + width  / 2.0f && mouse.x >= image_x - width  / 2.0f;
        bool within_y = mouse.y <= image_y + height / 2.0f && mouse.y >= image_y - height / 2.0f;

        if (within_x && within_y)
        {
            hovered_i = image_i;
            break;
        }
    }
    // Checks if mouse is hovering any image.
    // TODO We are not considering moving images, which use the offset thing.

    struct select *select            = &(images->select);
    bool           is_hovering       = hovered_i != -1;
    bool           is_multi          = INPUT_PRESSED == shift;
    bool           is_hover_selected = is_hovering && images->is_selecteds[hovered_i];

    enum select_action action = select_step(select, mouse.x, mouse.y, mouse.state, is_hovering, is_multi, is_hover_selected);

    if (SELECT_OFFSET_USE == action)
    {
        images->is_moving = true;
    }

    if (SELECT_OFFSET_APPLY == action)
    {
        for (int image_i = 0; image_i < images->n_image; image_i++)
        {
            if (images->is_selecteds[image_i])
            {
                images->xs[image_i] += images->select.offset_x;
                images->ys[image_i] += images->select.offset_y;
            }
        }
        images->is_moving = false;
    }

    if (SELECT_DESELECT_ALL == action || SELECT_DESELECT_ALL_EXCEPT == action)
    {
        for (int image_i = 0; image_i < images->n_image; image_i++)
        {
            images->is_selecteds[image_i] = false;
        }
        images->n_selected = 0;
    }

    if (SELECT_DESELECT_ALL_EXCEPT == action || SELECT_SELECT_TOO == action)
    {
        images->is_selecteds[hovered_i] = true;
        images->n_selected++;
    }

    if (SELECT_DESELECT_ONLY == action)
    {
        images->is_selecteds[hovered_i] = false;

        images->n_selected--;
    }

    return is_hovering;
}

void images_toggle(struct images *images, enum channel channel)
{
    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        if (images->is_selecteds[image_i])
        {
            switch (channel)
            {
                case CHANNEL_RED:   images->actives_red  [image_i] = !images->actives_red[image_i];   break;
                case CHANNEL_GREEN: images->actives_green[image_i] = !images->actives_green[image_i]; break;
                case CHANNEL_BLUE:  images->actives_blue [image_i] = !images->actives_blue[image_i];  break;
                default:            assert(!"Invalid channel!");                                      break;
            }
        }
    }

    images_compute(images);
}

void images_reset(struct images *images)
{
    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        if (images->is_selecteds[image_i])
        {
            images->actives_red  [image_i] = true;
            images->actives_green[image_i] = true;
            images->actives_blue [image_i] = true;
            images->grayscales   [image_i] = false;
            images->inverteds    [image_i] = false;
        }
    }

    images_compute(images);
}

void images_grayscale(struct images *images)
{
    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        if (images->is_selecteds[image_i])
        {
            images->grayscales[image_i] = !images->grayscales[image_i];
        }
    }

    images_compute(images);
}

void images_invert(struct images *images)
{
    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        if (images->is_selecteds[image_i])
        {
            images->inverteds[image_i] = !images->inverteds[image_i];
        }
    }

    images_compute(images);
}
