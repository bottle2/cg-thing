#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"
#include "safe.h"
#include "util.h"

void table_init(
    struct table   *table,
    int             n_column,
    int            capacity,
    int           *row_sizes,
    void         ***columns,
    bool            is_tracking_usage
) {
    assert(table     != NULL);
    assert(columns   != NULL);
    assert(row_sizes != NULL);

    assert(n_column >  0);
    assert(capacity >= 0);

    table->row_sizes = safe_malloc(n_column, sizeof (size_t   ));
    table->columns   = safe_malloc(n_column, sizeof (void   **));

    for (int column_i = 0; column_i < n_column; column_i++)
    {
        assert(row_sizes[column_i] > 0);
        assert(columns[column_i] != NULL);

        *(columns[column_i]) = NULL;

        table->row_sizes[column_i] = row_sizes[column_i];
        table->columns[column_i]   = columns[column_i];
    }

    table->n_column          = n_column;
    table->capacity          = 0;
    table->is_tracking_usage = is_tracking_usage;
    table->are_available     = NULL;

    table_ensure(table, capacity);
}

void table_free(struct table *table)
{
    assert(table != NULL);

    for (int column_i = 0; column_i < table->n_column; column_i++)
    {
        free(*(table->columns[column_i]));

        *(table->columns[column_i]) = NULL;
    }

    free(table->row_sizes);
    free(table->columns);
    free(table->are_available);

    table->n_column          = -1;
    table->capacity          = -1;
    table->is_tracking_usage = false;
    table->are_available     = NULL;
}

void table_ensure(struct table *table, int capacity)
{
    assert(table != NULL);
    assert(capacity >= 0);

    if (capacity > table->capacity)
    {
        int new_capacity = MAX_2(capacity, table->capacity * 2);
        // We double the capacity to avoid possible excessive `realloc()`ations.

        for (int column_i = 0; column_i < table->n_column; column_i++)
        {
            void   **column   = table->columns[column_i];
            size_t   row_size = table->row_sizes[column_i];

            *column = safe_realloc(*column, new_capacity, row_size);

            // We don't touch new rows since we don't know their "defaults".
        }

        if (table->is_tracking_usage)
        {
            table->are_available = (bool *)safe_realloc(
                table->are_available,
                new_capacity,
                sizeof (bool)
            );

            memset(
                table->are_available + table->capacity,
                1,
                new_capacity - table->capacity
            );
        }

        table->capacity = new_capacity;
    }
}

int table_next(struct table *table)
{
    assert(table != NULL);

    int available_i = table->capacity;
    // We already settle for the worst case and resize columns if needed later.

    for (int usage_i = 0; usage_i < table->capacity; usage_i++)
    {
        if (table->are_available[usage_i])
        {
            table->are_available[usage_i] = false;
            available_i = usage_i;
            break;
        }
    }

	table_ensure(table, available_i + 1);
    // Ensure that available row is inbounds, even if that means resizing all.

    return available_i;
}

void table_release(struct table *table, int row_i)
{
    assert(table != NULL);
    assert(row_i >= 0);
    assert(row_i < table->capacity);
    assert(false == table->are_available[row_i]);

    table->are_available[row_i] = true;
}
