#pragma once

#include "page_file.h"
#include "result.h"

class page_allocator_impl;

class page_allocator
{
public:
    page_allocator(page_file* page_file);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
private:
    page_allocator_impl* m_impl;
};
