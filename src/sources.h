#ifndef __T2__SOURCES__H__
#define __T2__SOURCES__H__

#include <stdint.h>

#include "expand.h"
#include "table.h"

#define SOURCES_TABLE_XS(A, X, S) \
X(A, char    *, pathnames       )S \
X(A, int      , widths          )S \
X(A, int      , heights         )S \
X(A, uint8_t *, per_image_reds  )S \
X(A, uint8_t *, per_image_greens)S \
X(A, uint8_t *, per_image_blues )

struct sources
{
    SOURCES_TABLE_XS(NOTHING, AS_COLUMN, SEMICOLON);
    struct table table;
};

void sources_init  (struct sources *sources);
void sources_free  (struct sources *sources);
int  sources_load  (struct sources *sources, char *pathname);
void sources_unload(struct sources *sources, int   source_i);

#endif // __T2__SOURCES__H__