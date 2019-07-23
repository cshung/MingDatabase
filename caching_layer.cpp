#include "caching_layer.h"
#include "allocator_layer.h"
#include "constant.h"

#include <unordered_map>
using namespace std;

struct cache_node
{
    cache_node();
    int m_page_number;
    void* m_buffer;
    bool m_is_dirty;
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
    result_t set_page_as_written(int page_number);
    result_t allocate_page(int* new_page_number);
    result_t deallocate_page(int page_number);
    result_t get_root(int* root);
    result_t set_root(int root);
    result_t close();
private:
    allocator_layer* m_allocator_layer;
    unordered_map<int, cache_node*> m_cache;
    cache_node m_front;
    cache_node m_back;
};

#include "caching_layer.forwarders.inl"

caching_layer_impl::caching_layer_impl()
{
    this->m_allocator_layer = new allocator_layer();
    this->m_front.m_next = &this->m_back;
    this->m_back.m_prev = &this->m_front;
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
    result_t result = result_t::success;
    auto probe = this->m_cache.find(page_number);
    cache_node* node = nullptr;
    if (probe == this->m_cache.end())
    {
        if (this->m_cache.size() < CACHE_SIZE)
        {
            node = new cache_node();
            node->m_page_number = page_number;
            node->m_buffer = new uint8_t[PAGE_SIZE];
            IfFailRet(this->m_allocator_layer->read_page(page_number, node->m_buffer));
        }
        else
        {
            node = this->m_front.m_next;

            if (node->m_is_dirty)
            {
                IfFailRet(this->m_allocator_layer->write_page(node->m_page_number, node->m_buffer));
                node->m_is_dirty = false;
            }

            this->m_cache.erase(node->m_page_number);

            // Remove the node from the linked list
            node->m_prev->m_next = node->m_next;
            node->m_next->m_prev = node->m_prev;

            node->m_page_number = page_number;
            IfFailRet(this->m_allocator_layer->read_page(page_number, node->m_buffer));
        }

        this->m_cache.insert(make_pair(page_number, node));
    }
    else
    {
        node = probe->second;

        // Remove the node from the linked list
        node->m_prev->m_next = node->m_next;
        node->m_next->m_prev = node->m_prev;
    }

    // Insert the node at the back of the queue - this is the most recently accessed node
    this->m_back.m_prev->m_next = node;
    node->m_prev = this->m_back.m_prev;
    m_back.m_prev = node;
    node->m_next = &m_back;

    *buffer = node->m_buffer;

    return result;
}

result_t caching_layer_impl::set_page_as_written(int page_number)
{
    result_t result = result_t::success;
    auto probe = this->m_cache.find(page_number);
    if (probe != this->m_cache.end())
    {
        probe->second->m_is_dirty = true;
    }

    return result;
}

result_t caching_layer_impl::allocate_page(int* new_page_number)
{
    return this->m_allocator_layer->allocate_page(new_page_number);
}

result_t caching_layer_impl::deallocate_page(int page_number)
{
    return this->m_allocator_layer->deallocate_page(page_number);
}


result_t caching_layer_impl::get_root(int* root)
{
    return this->m_allocator_layer->get_root(root);
}

result_t caching_layer_impl::set_root(int root)
{
    return this->m_allocator_layer->set_root(root);
}

result_t caching_layer_impl::close()
{
    result_t result = result_t::success;
    for (cache_node* p = this->m_front.m_next; p != &this->m_back; p = p->m_next)
    {
        if (p->m_is_dirty)
        {
            IfFailRet(this->m_allocator_layer->write_page(p->m_page_number, p->m_buffer));
        }
    }

    IfFailRet(this->m_allocator_layer->close());
    return result;
}

cache_node::cache_node()
{
    this->m_is_dirty = false;
    this->m_buffer = this->m_next = this->m_prev = nullptr;
}
