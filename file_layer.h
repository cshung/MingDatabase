#pragma once

#include "result.h"

class file_layer_impl;
class file_layer_listener;

class file_layer
{
public:
    file_layer();
    ~file_layer();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t append_page(int* new_page_number);
    result_t set_file_layer_listener(file_layer_listener* file_layer_listener);
    result_t get_num_pages(int* num_pages);
    result_t close();
private:
    file_layer_impl* m_impl;
};

class file_layer_listener
{
public:
    virtual result_t on_after_file_created() = 0;
    virtual result_t on_after_file_loaded() = 0;
    virtual result_t on_before_file_closed() = 0;
};
