#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "safe.h"
#include "sources.h"

void sources_init(struct sources *sources)
{
    assert(sources != NULL);

    for (int source_i = 0; source_i < SOURCES_CAPACITY; source_i++)
    {
        sources->widths          [source_i] = 0;
        sources->heights         [source_i] = 0;
        sources->names           [source_i] = NULL;
        sources->per_image_reds  [source_i] = NULL;
        sources->per_image_greens[source_i] = NULL;
        sources->per_image_blues [source_i] = NULL;
        sources->kinds           [source_i] = SOURCE_AVAILABLE;
    }
}

void sources_free(struct sources *sources)
{
    assert(sources != NULL);

    for (int source_i = 0; source_i < SOURCES_CAPACITY; source_i++)
    {
        if (sources->kinds[source_i] != SOURCE_AVAILABLE )
        {
            sources_unload(sources, source_i);
        }
    }
}

int sources_load(struct sources *sources, char *pathname)
{
    assert(sources  != NULL);
    assert(pathname != NULL);

    int source_i = -1;

    for (int candidate_i = 0; candidate_i < SOURCES_CAPACITY; candidate_i++)
    {
        enum source  kind = sources->kinds[candidate_i];
        char        *name = sources->names[candidate_i];

        if (SOURCE_FILE == kind && !strcmp(pathname, name))
        {
            source_i = candidate_i;
            break;
        }
        // We prioritize existing sources.

        if (-1 == source_i && SOURCE_AVAILABLE == kind)
        {
            source_i = candidate_i;
        }
        // Find first available source.
    }

    if (-1 == source_i)
    {
        return -1;
    }

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

    if (SOURCE_FILE == sources->kinds[source_i])
    {
        free(sources->per_image_reds  [source_i]);
        free(sources->per_image_greens[source_i]);
        free(sources->per_image_blues [source_i]);
    }
    else
    {
        sources->kinds[source_i] = SOURCE_FILE;
        sources->names[source_i] = safe_malloc(strlen(pathname) + 1, 1);

        memcpy(sources->names[source_i], pathname, strlen(pathname) + 1);
    }

    sources->heights         [source_i] = height;
    sources->widths          [source_i] = width;
    sources->per_image_reds  [source_i] = reds;
    sources->per_image_greens[source_i] = greens;
    sources->per_image_blues [source_i] = blues;

    return source_i;
}

void sources_unload(struct sources *sources, int source_i)
{
    assert(sources != NULL);
    assert(sources->kinds[source_i] != SOURCE_AVAILABLE);

    free(sources->per_image_reds  [source_i]);
    free(sources->per_image_greens[source_i]);
    free(sources->per_image_blues [source_i]);
    free(sources->names           [source_i]);

    sources->widths          [source_i] = 0;
    sources->heights         [source_i] = 0;
    sources->per_image_reds  [source_i] = NULL;
    sources->per_image_greens[source_i] = NULL;
    sources->per_image_blues [source_i] = NULL;
    sources->names           [source_i] = NULL;

    sources->kinds[source_i] = SOURCE_AVAILABLE;
}