#pragma once

#include "result.h"
#include "caching_layer.h"

struct buffer
{
    void* data;
    int size;
};

class btree_impl;

class btree
{
public:
    btree(caching_layer* cache);
    result_t insert(buffer key, buffer value);
private:
    btree_impl* m_impl;
};
