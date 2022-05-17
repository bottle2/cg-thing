#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "safe.h"
#include "sources.h"

void sources_init(struct sources *sources)
{
    assert(sources != NULL);

    int    n_column    =   SOURCES_TABLE_XS(NOTHING  , AS_ONE      , SUM  )  ;
    int    row_sizes[] = { SOURCES_TABLE_XS(NOTHING  , AS_SIZE     , COMMA) };
    void **columns[]   = { SOURCES_TABLE_XS(sources->, AS_ADDRESSES, COMMA) };

    table_init(&(sources->table), n_column, 0, row_sizes, columns, true);
}

void sources_free(struct sources *sources)
{
    assert(sources != NULL);

    for (int source_i = 0; source_i < sources->table.capacity; source_i++)
    {
        if (!(sources->table.are_available[source_i]))
        {
            free(sources->per_image_reds[source_i]);
            free(sources->per_image_greens[source_i]);
            free(sources->per_image_blues[source_i]);
            free(sources->pathnames[source_i]);

            table_release(&(sources->table), source_i);
        }
    }

    table_free(&(sources->table));
}

int sources_load(struct sources *sources, char *pathname)
{
    assert(sources  != NULL);
    assert(pathname != NULL);

    int source_i = table_next(&(sources->table));

    int            width   = 0;
    int            height  = 0;
    unsigned char *data    = bmp_get(pathname, &width, &height);
    int            n_pixel = width * height;

    if (NULL == data)
    {
        return -1;
    }

    uint8_t *reds   = safe_malloc(n_pixel, sizeof(uint8_t));
    uint8_t *greens = safe_malloc(n_pixel, sizeof(uint8_t));
    uint8_t *blues  = safe_malloc(n_pixel, sizeof(uint8_t));

    for (int pixel_i = 0; pixel_i < n_pixel; pixel_i++)
    {
        reds  [pixel_i] = data[pixel_i * 3 + 2];
        greens[pixel_i] = data[pixel_i * 3 + 1];
        blues [pixel_i] = data[pixel_i * 3 + 0];
        // DIB stores pixel as BGR instead of RGB.
    }
    // Read pixels from image.

    free(data);

    sources->per_image_reds  [source_i] = reds;
    sources->per_image_greens[source_i] = greens;
    sources->per_image_blues [source_i] = blues;
    sources->heights         [source_i] = height;
    sources->widths          [source_i] = width;

    sources->pathnames[source_i] = safe_malloc(strlen(pathname) + 1, 1);
    memcpy(sources->pathnames[source_i], pathname, strlen(pathname) + 1);

    return source_i;
}

void sources_unload(struct sources *sources, int source_i)
{
    assert(sources != NULL);

    table_release(&(sources->table), source_i);

    free(sources->per_image_reds  [source_i]);
    free(sources->per_image_greens[source_i]);
    free(sources->per_image_blues [source_i]);
    free(sources->pathnames       [source_i]);

    sources->widths [source_i] = 0;
    sources->heights[source_i] = 0;
    sources->per_image_reds   = NULL;
    sources->per_image_greens = NULL;
    sources->per_image_blues  = NULL;
    sources->pathnames        = NULL;
}