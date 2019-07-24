#pragma once
#include <stdint.h>

/**
 * This represents a buffer with a size prefix.
 */
struct buffer
{
    /**
     * This size of the buffer.
     */
    uint16_t size;

    /**
     * The data payload with `size` number of bytes.
     */
    void* data;
};
