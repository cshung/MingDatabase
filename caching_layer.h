#pragma once

#include "result.h"

class caching_layer_impl;

class caching_layer
{
public:
    caching_layer();
    ~caching_layer();
    result_t open(const char* file_name);
    result_t get_page(int page_number, void** buffer);
    result_t set_page_as_written(int page_number);
    result_t allocate_page(int* new_page_number);
    result_t deallocate_page(int page_number);
    result_t get_root(int* root);
    result_t set_root(int root);
    result_t close();
private:
    caching_layer_impl* m_impl;
};
