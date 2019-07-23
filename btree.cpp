#include "btree.h"
#include "constant.h"

class btree_impl
{
public:
    btree_impl(caching_layer* caching_layer);
    result_t initialize();
    result_t insert(buffer key, buffer value);
    result_t close();
private:
    caching_layer* m_cache;
    int m_root;
};

#include "btree.forwarders.inl"

struct node_header
{
    bool is_leaf;
    int num_keys;
};

btree_impl::btree_impl(caching_layer* cache)
{
    this->m_cache = cache;
}

result_t btree_impl::initialize()
{
    result_t result = result_t::success;
    IfFailRet(this->m_cache->get_root(&this->m_root));
    if (this->m_root == 0)
    {
        IfFailRet(this->m_cache->allocate_page(&this->m_root));
        node_header* root_node;
        IfFailRet(this->m_cache->get_page(this->m_root, (void**)&root_node));
        root_node->is_leaf = true;
        root_node->num_keys = 0;
        this->m_cache->set_page_as_written(this->m_root);
    }
}

result_t btree_impl::insert(buffer key, buffer value)
{
    result_t result = result_t::success;

    // TODO: Implementation
    return result;
}

result_t btree_impl::close()
{
    return this->m_cache->close();
}
