#pragma once

#include "page_file.h"
#include "result.h"

class page_allocator_impl;

class page_allocator
{
public:
    page_allocator();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t allocate_page(int* new_page_number);
    result_t close();
private:
    page_allocator_impl* m_impl;
};
