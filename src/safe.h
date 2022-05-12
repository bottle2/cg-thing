#ifndef __T1__SAFE__H__
#define __T1__SAFE__H__

// Handles errors for you, use the functions unworried!

#include <stddef.h>
#include <stdio.h>

void * safe_malloc (           size_t nmemb, size_t size);
void * safe_realloc(void *ptr, size_t nmemb, size_t size);

FILE * safe_fopen(char const *pathname, char const *mode);
// Returns NULL if pathname is "wrong".

size_t safe_get_file(char const *pathname, char **buffer);
// Don't forget to free the buffer!
// `buffer` is set to NULL if file didn't exist or is too big. Returns -1 if so.

#endif // __T1__SAFE__H__
