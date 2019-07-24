#pragma once

#include "result.h"
#include "caching_layer.h"

struct buffer
{
    int size;
    void* data;
};

class btree_impl;

class btree
{
public:
    btree(caching_layer* caching_layer, int root);
    result_t initialize();
    result_t insert(buffer key, buffer value);
    result_t close();
private:
    btree_impl* m_impl;
};
