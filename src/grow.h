#ifndef __T2__GROW__H__
#define __T2__GROW__H__

// Handles growth of arrays with an one-to-one association among their elements.
// We call this inter-array association "rows".

#include <stdbool.h>
#include <stddef.h>

struct grow
{
    int       n_array;
    size_t    capacity;
    size_t   *element_sizes;
    void   ***arrays;
    // Notice that `realloc()` may change pointers.
    // This explains using an array of pointers to pointers. 

    bool  is_tracking_usage;
    bool *are_available;
    // Convenient agnostic tracking of which rows are in use.
};

void grow_init(
    struct grow   *grow,
    int            n_array,
    size_t         capacity,
    size_t        *element_sizes,
    void        ***arrays,
    bool           is_tracking_usage
);
// Use X Macros to generate `element_sizes` and `arrays`. Check `expand.h`.

void grow_free(struct grow *grow);
// Storing pointers to the address of each array also let us set them to NULL!

void grow_ensure(struct grow* grow, size_t capacity);
// Checks if access of arrays won't be out of bounds, and resizes if so.
// Doubles `capacity` if new `capacity` doesn't far exceed it.

int  grow_next   (struct grow *grow);
void grow_release(struct grow *grow, int element_i);
// Gives you the next available index, which can be later released for reuse.
// Use these if you init'ed `grow` with `is_tracking_usage` to `true`.
// `element_i` MUST be a value previously returned by `grow_next()`.

#endif