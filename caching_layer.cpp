#pragma once

#include "caching_layer.h"
#include "allocator_layer.h"

#include <unordered_map>
using namespace std;

struct cache_node
{
    cache_node();
    int page_number;
    void* m_buffer;
    cache_node* m_prev;
    cache_node* m_next;
};

class caching_layer_impl
{
public:
    caching_layer_impl();
    ~caching_layer_impl();
    result_t open(const char* file_name);
    result_t get_page(int page_number, void** buffer);
    result_t put_page(int page_number, void** buffer, bool is_written);
    result_t allocate_page(int* new_page_number);
    result_t deallocate_page(int page_number);
    result_t close();
private:
    allocator_layer* m_allocator_layer;
    unordered_map<int, cache_node*> m_cache;
    cache_node m_clean_front;
    cache_node m_clean_back;
    cache_node m_dirty_front;
    cache_node m_dirty_back;
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
    return this->m_allocator_layer->open(file_name);
}

result_t caching_layer_impl::get_page(int page_number, void** buffer)
{
    return result_t::not_implemented;
}

result_t caching_layer_impl::put_page(int page_number, void** buffer, bool is_written)
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
    return this->m_allocator_layer->close();
}

cache_node::cache_node()
{
    this->m_buffer = this->m_next = this->m_prev = nullptr;
}
