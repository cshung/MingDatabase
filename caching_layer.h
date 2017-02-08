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
    result_t put_page(int page_number, void** buffer, bool is_written);
    result_t allocate_page(int* new_page_number);
    result_t deallocate_page(int page_number);
    result_t close();
private:
    caching_layer_impl* m_impl;
};
