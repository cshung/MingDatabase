#include "btree.h"
#include "constant.h"

class btree_impl
{
public:
    btree_impl(caching_layer* caching_layer);
    result_t insert(buffer key, buffer value);
private:
    caching_layer* m_cache;
    int m_root;
};

#include "btree.forwarders.inl"

struct page_header
{
    bool is_leaf;
    int begin;
    int end;
};

btree_impl::btree_impl(caching_layer* cache)
{
    this->m_cache = cache;

    // Creating an empty leaf page
    this->m_cache->allocate_page(&this->m_root);
    void* buffer;
    this->m_cache->get_page(this->m_root, &buffer);
    page_header* header = (page_header*)buffer;
    header->is_leaf = true;
    header->begin = sizeof(page_header);
    header->end = PAGE_SIZE;
    this->m_cache->set_page_as_written(this->m_root);
}

result_t btree_impl::insert(buffer key, buffer value)
{
    result_t result = result_t::success;
    return result;
}
