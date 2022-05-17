#ifndef __T2__SOURCES__H__
#define __T2__SOURCES__H__

#include <stdint.h>

#define SOURCES_CAPACITY 256

enum source { SOURCE_AVAILABLE, SOURCE_FILE, SOURCE_PROCEDURAL, };

struct sources
{
    char        *names           [SOURCES_CAPACITY];
    int          widths          [SOURCES_CAPACITY];
    int          heights         [SOURCES_CAPACITY];
    uint8_t     *per_image_reds  [SOURCES_CAPACITY];
    uint8_t     *per_image_greens[SOURCES_CAPACITY];
    uint8_t     *per_image_blues [SOURCES_CAPACITY];
    enum source  kinds           [SOURCES_CAPACITY];
};

void sources_init  (struct sources *sources);
void sources_free  (struct sources *sources);
int  sources_load  (struct sources *sources, char *pathname);
void sources_unload(struct sources *sources, int   source_i);

#endif // __T2__SOURCES__H__