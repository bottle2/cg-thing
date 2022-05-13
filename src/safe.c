#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "safe.h"

#define FILE_READ_BUFFER 10000

static inline bool check_has_overflow_product(size_t a, size_t b)
{
    return a > ~((size_t) 0) / b;
}

void * safe_malloc(size_t nmemb, size_t size)
{
    assert(!check_has_overflow_product(nmemb, size));

    void *ptr = malloc(nmemb * size);

    if (NULL == ptr)
    {
        fprintf(stderr, "Your computer doesn't has enough memory for us!\n");
    }

    return ptr;
}

void * safe_realloc(void *ptr, size_t nmemb, size_t size)
{
    assert(!check_has_overflow_product(nmemb, size));

    void *ptr_new = realloc(ptr, nmemb * size);

    if (NULL == ptr_new)
    {
        fprintf(stderr, "Your computer doesn't has enough memory for us!\n");
        exit(EXIT_FAILURE);
    }

    return ptr_new;

    return NULL;
}

// We assert for too much memory because it is a programming error to even allow
// huge allocations, since some DoS attack or exploit could be in course.

FILE * safe_fopen(char const *pathname, char const *mode)
{
    errno = 0;

    FILE *file = fopen(pathname, mode);

    int error = errno;

    assert(error != EINVAL);

    if (NULL == file)
    {
        if (ENOMEM == error)
        {
            fprintf(stderr, "Your computer doesn't has enough memory for us!\n");
            exit(EXIT_FAILURE);
        }
        else // Probably something wrong with `pathname`. TODO check `errno`.
        {
            return NULL;
        }
    }

    return file;
}

size_t safe_get_file(char const *pathname, char **buffer)
{
    assert(buffer  != NULL);
    assert(*buffer == NULL);

    size_t  total_bytes = 0;
    FILE   *file        = safe_fopen(pathname, "rb");
    char   *read_buffer[FILE_READ_BUFFER];

    while (!feof(file))
    {
        size_t read_bytes = fread(read_buffer, 1, FILE_READ_BUFFER, file);

        if (ferror(file))
        {
            clearerr(file);
            continue;
        }

        total_bytes += read_bytes;

        char *buffer_new = (char *)realloc(*buffer, total_bytes);

        if (NULL == buffer_new)
        {
            free(*buffer);
            *buffer = NULL;
            return -1;
        }

        *buffer = buffer_new;

        memcpy(*buffer + total_bytes - read_bytes, read_buffer, read_bytes);
    }

    (void)fclose(file);

    return total_bytes;
}


















