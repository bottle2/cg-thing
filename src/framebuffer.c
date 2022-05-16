#include <assert.h>
#include <string.h>

#include "gl_canvas2d.h"

#include "expand.h"
#include "framebuffer.h"

void framebuffer_init(struct framebuffer *framebuffer, int width, int height)
{
    assert(framebuffer != NULL);
    assert(width  > 0);
    assert(height > 0);

    int    n_column    =   FRAMEBUFFER_TABLE_XS(NOTHING      , AS_ONE      , SUM  )  ;
    int    row_sizes[] = { FRAMEBUFFER_TABLE_XS(NOTHING      , AS_SIZE     , COMMA) };
    void **columns[]   = { FRAMEBUFFER_TABLE_XS(framebuffer->, AS_ADDRESSES, COMMA) };

    table_init(&(framebuffer->table), n_column, width * height, row_sizes, columns, false);

    framebuffer->width  = width;
    framebuffer->height = height;

    framebuffer_clear(framebuffer);
}

void framebuffer_free(struct framebuffer *framebuffer)
{
    assert(framebuffer != NULL);

    table_free(&(framebuffer->table));
}

void framebuffer_resize(struct framebuffer *framebuffer, int width, int height)
{
    assert(framebuffer != NULL);
    assert(width  > 0);
    assert(height > 0);

    struct framebuffer new_framebuffer = { 0 };

    framebuffer_init(&new_framebuffer, width, height);

    framebuffer_burn(
        &new_framebuffer,
        framebuffer->width,
        framebuffer->height,
        framebuffer->width / 2,
        framebuffer->height / 2,
        framebuffer->reds,
        framebuffer->greens,
        framebuffer->blues
	);

    table_free(&(framebuffer->table));

    *framebuffer = new_framebuffer;
}

void framebuffer_clear(struct framebuffer *framebuffer)
{
    assert(framebuffer != NULL);

    int n_pixel = framebuffer->width * framebuffer->height;

    memset(framebuffer->z_buffer, false, sizeof(bool) * n_pixel);
}

void framebuffer_burn(
    struct framebuffer *framebuffer,
    int                 width,
    int                 height,
    int                 x,
    int                 y,
    float              *reds,
    float              *greens,
    float              *blues
) {
    assert(framebuffer != NULL);
    assert(reds   != NULL);
    assert(greens != NULL);
    assert(blues  != NULL);

    int n_pixel = width * height;

    for (int pixel_i = 0; pixel_i < n_pixel; pixel_i++)
    {
        int source_x      = pixel_i % width;
        int source_y      = pixel_i / width;
        int framebuffer_x = source_x + x;
        int framebuffer_y = source_y + y;

        int framebuffer_pixel_i = framebuffer_x + framebuffer_y * framebuffer->width;

        if (
               framebuffer_x >= 0
            && framebuffer_x <  framebuffer->width
            && framebuffer_y >= 0
            && framebuffer_y <  framebuffer->height
            && false == framebuffer->z_buffer[framebuffer_pixel_i]
        ) {
			framebuffer->reds[  framebuffer_pixel_i]   = reds  [pixel_i];
			framebuffer->greens[framebuffer_pixel_i]   = greens[pixel_i];
			framebuffer->blues[ framebuffer_pixel_i]   = blues [pixel_i];
            framebuffer->z_buffer[framebuffer_pixel_i] = true;
        }
        // XXX Consider CPU bubbling of an `if` clause in a tight loop.
    }
}

void framebuffer_render(struct framebuffer *framebuffer)
{
    assert(framebuffer != NULL);

    int    n_pixel  = framebuffer->width * framebuffer->height;
    float *reds     = framebuffer->reds;
    float *greens   = framebuffer->greens;
    float *blues    = framebuffer->blues;
    bool  *z_buffer = framebuffer->z_buffer;

    assert(reds   != NULL);
    assert(greens != NULL);
    assert(blues  != NULL);

    for (int pixel_i = 0; pixel_i < n_pixel; pixel_i++)
    {
        if (true == z_buffer[pixel_i])
        {
			int x = pixel_i % framebuffer->width;
			int y = pixel_i / framebuffer->width;

			cv_color_rgb(reds[pixel_i], greens[pixel_i], blues[pixel_i]);
			cv_rectFill((float)x, (float)y, (float)(x + 1), (float)(y + 1));
        }
        // XXX Consider CPU bubbling of an `if` clause in a tight loop.
    }
}
