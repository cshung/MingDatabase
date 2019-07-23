#pragma once

#include "result.h"

class allocator_layer_impl;

/**
 * This class represents the interface of the allocator layer.
 */
class allocator_layer
{
public:
    allocator_layer();
    ~allocator_layer();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t get_root(int* root);
    result_t set_root(int root);
    result_t allocate_page(int* new_page_number);
    result_t deallocate_page(int page_number);
    result_t close();
private:
    allocator_layer_impl* m_impl;
};
