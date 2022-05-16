#ifndef __T2__TABLE__H__
#define __T2__TABLE__H__

// Consider parallel arrays as columns of a table.
// This module creates and grows tables.

#include <stdbool.h>

struct table
{
    int     n_column;
    int     capacity;
    int    *row_sizes;
    void ***columns;
    // Notice that `realloc()` may change pointers.
    // This explains using an array of pointers to pointers. 

    bool  is_tracking_usage;
    bool *are_available;
    // Convenient agnostic tracking of which rows are in use.
};

void table_init(
    struct table   *table,
    int             n_column,
    int            capacity,
    int            *row_sizes,
    void         ***columns,
    bool            is_tracking_usage
);
// Use X Macros to generate `row_sizes` and `columns`. Check `expand.h`.

void table_free(struct table *table);
// Storing pointers to the address of each column also let us set them to NULL!

void table_ensure(struct table* table, int capacity);
// Checks if access of rows won't be out of bounds, and resizes if so.
// Doubles `capacity` if new `capacity` doesn't far exceed it.

int  table_next   (struct table *table);
void table_release(struct table *table, int row_i);
// Returns the next available row, which can be later released for reuse.
// Use these if `table` was `table_init()`ed with `is_tracking_usage` to `true`.
// `row_i` MUST be a value previously returned by `table_next()`.

#endif