#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "gl_canvas2d.h"

#include "bmp.h"
#include "framebuffer.h"
#include "images.h"
#include "safe.h"
#include "select.h"

void images_init(struct images *images)
{
    assert(images != NULL);

    images->n_image                 = 0;
    images->n_selected              = 0;
    images->is_moving               = false;

    select_init(&(images->select));
}

int images_load(struct images *images, char const *pathname, int x, int y)
{
    assert(images   != NULL);
    assert(pathname != NULL);
    assert(images->n_image < IMAGE_CAPACITY);

    int image_n = images->n_image;

    int            width   = 0;
    int            height  = 0;
    unsigned char *data    = bmp_get(pathname, &width, &height);
    int            n_pixel = width * height;

    if (NULL == data)
    {
        return -1;
    }

    uint8_t *reds         = (uint8_t *)safe_malloc(n_pixel, 1);
    uint8_t *greens       = (uint8_t *)safe_malloc(n_pixel, 1);
    uint8_t *blues        = (uint8_t *)safe_malloc(n_pixel, 1);
    float   *cache_reds   = (float   *)safe_malloc(n_pixel, sizeof (float));
    float   *cache_greens = (float   *)safe_malloc(n_pixel, sizeof (float));
    float   *cache_blues  = (float   *)safe_malloc(n_pixel, sizeof (float));


    for (int pixel_i = 0; pixel_i < n_pixel; pixel_i++)
    {
        reds[pixel_i]   = data[pixel_i * 3 + 2];
        greens[pixel_i] = data[pixel_i * 3 + 1];
        blues[pixel_i]  = data[pixel_i * 3 + 0];
        // DIB stores pixel as BGR instead of RGB.
    }
    // Read pixels from image.

    free(data);

    images->n_image++;

    images->per_image_reds[image_n]         = reds;
    images->per_image_greens[image_n]       = greens;
    images->per_image_blues[image_n]        = blues;
    images->per_image_cache_reds[image_n]   = cache_reds;
    images->per_image_cache_greens[image_n] = cache_greens;
    images->per_image_cache_blues[image_n]  = cache_blues;

    images->actives_red  [image_n] = true;
    images->actives_green[image_n] = true;
    images->actives_blue [image_n] = true;

    images->widths[image_n]  = width;
    images->heights[image_n] = height;
    images->xs[image_n]      = x;
    images->ys[image_n]      = y;

    images->n_selected++;
    images->is_selecteds[image_n] = true;
    images_compute(images);

    images->pathnames[image_n] = (char *)safe_malloc(strlen(pathname) + 1, 1);
    memcpy(images->pathnames[image_n], pathname, strlen(pathname) + 1);

    return 0;
}

void images_compute(struct images *images)
{
    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        if (images->is_selecteds[image_i])
        {
            int      width   = images->widths [image_i];
            int      height  = images->heights[image_i];
            int      n_pixel = width * height;
            uint8_t *reds    = images->per_image_reds  [image_i];
            uint8_t *greens  = images->per_image_greens[image_i];
            uint8_t *blues   = images->per_image_blues [image_i];
            float   *cache_reds   = images->per_image_cache_reds  [image_i];
            float   *cache_greens = images->per_image_cache_greens[image_i];
            float   *cache_blues  = images->per_image_cache_blues [image_i];
            bool     active_red   = images->actives_red[image_i];
            bool     active_green = images->actives_green[image_i];
            bool     active_blue  = images->actives_blue[image_i];
            bool     is_grayscale = images->grayscales[image_i];
            bool     is_inverted  = images->inverteds[image_i];

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
                cache_reds[pixel_i]   = active_red   ? (float)reds[pixel_i]   / 255.0f : 0.0f;
                cache_greens[pixel_i] = active_green ? (float)greens[pixel_i] / 255.0f : 0.0f;
                cache_blues[pixel_i]  = active_blue  ? (float)blues[pixel_i]  / 255.0f : 0.0f;

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

void images_render(struct images *images, int screen_width, int screen_height)
{
    assert(images != NULL);
    assert(images->n_image >= 0);

    int offset_x = images->is_moving ? images->select.offset_x : 0;
    int offset_y = images->is_moving ? images->select.offset_y : 0;

    struct framebuffer framebuffer = { 0 };

    framebuffer_init(&framebuffer, screen_width, screen_height);

#if 0
    for (int image_i = 0; image_i < images->n_image; image_i++)
#else
    for (int image_i = images->n_image; image_i >= 0; image_i--)
#endif
    {
        float *reds   = images->per_image_cache_reds[image_i];
        float *greens = images->per_image_cache_greens[image_i];
        float *blues  = images->per_image_cache_blues[image_i];

        int    width  = images->widths[image_i];
        int    height = images->heights[image_i];
        int    x      = images->xs[image_i] - images->widths[image_i]  / 2;
        int    y      = images->ys[image_i] - images->heights[image_i] / 2;
        bool   is_selected = images->is_selecteds[image_i];

        assert(reds   != NULL);
        assert(greens != NULL);
        assert(blues  != NULL);
        assert(width  > 0);
        assert(height > 0);

#if 0
        for (int pixel_i = 0; pixel_i < width * height; pixel_i++)
        {
            int pixel_x = pixel_i % width;
            int pixel_y = pixel_i / width;

            cv_color_rgba(reds[pixel_i], greens[pixel_i], blues[pixel_i], 20.0f);
            cv_rectFill(
                (float)(pixel_x + x     + (is_selected ? offset_x : 0)),
                (float)(pixel_y + y     + (is_selected ? offset_y : 0)),
                (float)(pixel_x + x + 1 + (is_selected ? offset_x : 0)),
                (float)(pixel_y + y + 1 + (is_selected ? offset_y : 0))
            );
        }
#else
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
#endif
    }

    framebuffer_render(&framebuffer);
    framebuffer_free(&framebuffer);

    for (int image_i = 0; image_i < images->n_image; image_i++)
    {
        int    width  = images->widths[image_i];
        int    height = images->heights[image_i];
        int    x      = images->xs[image_i] - images->widths[image_i]  / 2;
        int    y      = images->ys[image_i] - images->heights[image_i] / 2;
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

        free(images->per_image_reds  [image_i]);
        free(images->per_image_greens[image_i]);
        free(images->per_image_blues [image_i]);

        free(images->per_image_cache_reds  [image_i]);
        free(images->per_image_cache_greens[image_i]);
        free(images->per_image_cache_blues [image_i]);
    }

    images->n_image = 0;
}

int images_trace(struct images *images, int x, int y)
{
    images->hovered_i = -1;

    for (int image_i = images->n_image; image_i >= 0; image_i--)
    {
        int width   = images->widths[image_i];
        int height  = images->heights[image_i];
        int image_x = images->xs[image_i];
        int image_y = images->ys[image_i];

        bool within_x = x <= image_x + width  / 2.0f && x >= image_x - width  / 2.0f;
        bool within_y = y <= image_y + height / 2.0f && y >= image_y - height / 2.0f;

        if (within_x && within_y)
        {
            images->hovered_i = image_i;
            break;
        }
    }
    return images->hovered_i;
}

void images_click(
    struct images    *images,
    int               x,
    int               y,
    enum input_state  state,
    bool              is_multi
) {
    enum select_action action = select_step(
        &(images->select),
        x,
        y,
        state, images->hovered_i != -1, is_multi,
        images->is_selecteds[images->hovered_i]
    );

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
        images->is_selecteds[images->hovered_i] = true;
        images->n_selected++;
    }

    if (SELECT_DESELECT_ONLY == action)
    {
        images->is_selecteds[images->hovered_i] = false;

        images->n_selected--;
    }
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
                default:            assert(!"Invalid channel!");                    break;
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
