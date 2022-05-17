#ifndef __T2__FRAMEBUFFER__H__
#define __T2__FRAMEBUFFER__H__

#include <stdbool.h>

struct framebuffer
{
    int    width;
    int    height;
    float *reds;
    float *greens;
    float *blues;
    bool  *z_buffer;
};
// Pixels are stored from bottom-left to upper-right.

void framebuffer_init  (struct framebuffer *framebuffer, int width, int height);
void framebuffer_free  (struct framebuffer *framebuffer);
void framebuffer_resize(struct framebuffer *framebuffer, int width, int height);
void framebuffer_clear (struct framebuffer *framebuffer);
void framebuffer_render(struct framebuffer *framebuffer);

void framebuffer_burn(
    struct framebuffer *framebuffer,
    int                 width,
    int                 height,
    int                 x,
    int                 y,
    float              *reds,
    float              *greens,
    float              *blues
);
// Must feed topmost images first.
// Expects x and y from bottom-left corner of image.

#endif