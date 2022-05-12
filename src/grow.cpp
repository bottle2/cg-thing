#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "grow.h"
#include "safe.h"
#include "util.h"

void grow_init(
    struct grow   *grow,
    int            n_array,
    size_t         capacity,
    size_t        *element_sizes,
    void        ***arrays,
    bool           is_tracking_usage
) {
    assert(grow          != NULL);
    assert(arrays        != NULL);
    assert(element_sizes != NULL);

    assert(n_array  >  0);
    assert(capacity >= 0);

    grow->element_sizes = (size_t   *)safe_malloc(n_array, sizeof (size_t   ));
    grow->arrays        = (void   ***)safe_malloc(n_array, sizeof (void   **));

    for (int array_i = 0; array_i < n_array; array_i++)
    {
        assert(element_sizes[array_i] > 0);
        assert(arrays[array_i] != NULL);

        *(arrays[array_i]) = NULL;

        grow->element_sizes[array_i] = element_sizes[array_i];
        grow->arrays[array_i]        = arrays[array_i];
    }

    grow->n_array           = n_array;
    grow->capacity          = 0;
    grow->is_tracking_usage = is_tracking_usage;
    grow->are_available     = NULL;

    grow_ensure(grow, capacity);
}

void grow_free(struct grow *grow)
{
    assert(grow != NULL);

    for (int array_i = 0; array_i < grow->n_array; array_i++)
    {
        free(*(grow->arrays[array_i]));

        *(grow->arrays[array_i]) = NULL;
    }

    free(grow->element_sizes);
    free(grow->arrays);
    free(grow->are_available);

    grow->n_array           = -1;
    grow->capacity          = -1;
    grow->is_tracking_usage = false;
    grow->are_available     = NULL;
}

void grow_ensure(struct grow *grow, size_t capacity)
{
    assert(grow != NULL);
    assert(capacity >= 0);

    if (capacity > grow->capacity)
    {
        size_t new_capacity = MAX_2(capacity, grow->capacity * 2);
        // We double the capacity to avoid possible excessive `realloc()`ations.

        for (int array_i = 0; array_i < grow->n_array; array_i++)
        {
            void   **array        = grow->arrays[array_i];
            size_t   element_size = grow->element_sizes[array_i];

            *array = safe_realloc(*array, new_capacity, element_size);

            // We don't touch new elements since we don't know their "defaults".
        }

        if (grow->is_tracking_usage)
        {
            grow->are_available = (bool *)safe_realloc(
                grow->are_available,
                new_capacity,
                sizeof (bool)
            );

            memset(
                grow->are_available + grow->capacity,
                1,
                new_capacity - grow->capacity
            );
        }

        grow->capacity = new_capacity;
    }
}

int grow_next(struct grow *grow)
{
    assert(grow != NULL);

    int available_i = grow->capacity;
    // We already settle for the worst case and resize arrays if needed later.

    for (int usage_i = 0; usage_i < grow->capacity; usage_i++)
    {
        if (grow->are_available[usage_i])
        {
            grow->are_available[usage_i] = false;
            available_i = usage_i;
            break;
        }
    }

	grow_ensure(grow, available_i + 1);
    // Ensure that available row is inbounds, even if that means resizing all.

    return available_i;
}

void grow_release(struct grow *grow, int element_i)
{
    assert(grow != NULL);
    assert(element_i >= 0);
    assert(element_i < grow->capacity);
    assert(false == grow->are_available[element_i]);

    grow->are_available[element_i] = true;
}
