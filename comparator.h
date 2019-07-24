#pragma once

#include "buffer.h"
#include "result.h"

/**
 * This class represents the functionality of comparing two buffers.
 */
class comparator
{
public:
    /**
     * Compare two buffers.
     * @param key1 The first key to compare.
     * @param key2 The second key to compare.
     * @param answer [out] A number that abstractly represent key1 - key2.
     * @return result_t::success if the keys are sucessfully compared.
     */
    virtual result_t compare(buffer key1, buffer key2, int* answer) = 0;
};

/**
 * The class represents the functionality to compare two buffers that represents strings.
 */
class string_comparator : public comparator
{
public:
    /**
     * Compare two buffers.
     * @param key1 The first key to compare.
     * @param key2 The second key to compare.
     * @param answer [out] A number that abstractly represent key1 - key2.
     * @return result_t::success if the keys are sucessfully compared.
     */
    virtual result_t compare(buffer key1, buffer key2, int* answer);
};
