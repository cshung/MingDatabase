#pragma once

#include "result.h"
#include "caching_layer.h"
#include "comparator.h"

class btree_impl;

/**
 * The class represents a B tree.
 */
class btree
{
public:
    btree(caching_layer* caching_layer, comparator* comparator, int root);
    result_t initialize();
    result_t insert(buffer key, buffer value);
private:
    btree_impl* m_impl;
};