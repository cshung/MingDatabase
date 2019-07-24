#pragma once

/**
 * This represents a buffer with a size prefix.
 */
struct buffer
{
    /**
     * This size of the buffer.
     */
    int size;

    /**
     * The data payload with `size` number of bytes.
     */
    void* data;
};
