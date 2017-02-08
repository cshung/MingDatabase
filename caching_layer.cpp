#pragma once

#include "caching_layer.h"
#include "allocator_layer.h"

class caching_layer_impl
{
public:
    caching_layer_impl();
    ~caching_layer_impl();
    result_t open(const char* file_name);
    result_t get_page(int page_number, void** buffer);
    result_t mark_page_as_written(int page_number);
    result_t allocate_page(int* new_page_number);
    result_t deallocate_page(int page_number);
    result_t close();
private:
    allocator_layer* m_allocator_layer;
};

#include "caching_layer.forwarders.inl"

caching_layer_impl::caching_layer_impl()
{
    this->m_allocator_layer = new allocator_layer();
}

caching_layer_impl::~caching_layer_impl()
{
    delete this->m_allocator_layer;
}

result_t caching_layer_impl::open(const char* file_name)
{
    return result_t::not_implemented;
}

result_t caching_layer_impl::get_page(int page_number, void** buffer)
{
    return result_t::not_implemented;
}

result_t caching_layer_impl::mark_page_as_written(int page_number)
{
    return result_t::not_implemented;
}

result_t caching_layer_impl::allocate_page(int* new_page_number)
{
    return result_t::not_implemented;
}

result_t caching_layer_impl::deallocate_page(int page_number)
{
    return result_t::not_implemented;
}

result_t caching_layer_impl::close()
{
    return result_t::not_implemented;
}
