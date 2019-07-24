#include "btree.h"
#include "constant.h"

class btree_impl
{
public:
    btree_impl(caching_layer* caching_layer, int root);
    result_t initialize();
    result_t insert(buffer key, buffer value);
    result_t close();
private:
    result_t insert(int node, buffer key, buffer value);
    result_t leaf_insert(void* node_memory, buffer key, buffer value);
    result_t internal_insert(void* node_memory, buffer key, buffer value);
    caching_layer* m_caching_layer;
    int m_root;
};

#include "btree.forwarders.inl"

struct node_header
{
    bool is_leaf;
    int num_keys;
};

btree_impl::btree_impl(caching_layer* caching_layer, int root)
{
    this->m_caching_layer = caching_layer;
    this->m_root = root;
}

result_t btree_impl::initialize()
{
    result_t result = result_t::success;
    if (this->m_root == 0)
    {
        IfFailRet(this->m_caching_layer->allocate_page(&this->m_root));
        node_header* root_node;
        IfFailRet(this->m_caching_layer->get_page(this->m_root, (void**)&root_node));
        root_node->is_leaf = true;
        root_node->num_keys = 0;
        this->m_caching_layer->set_page_as_written(this->m_root);
    }
}

result_t btree_impl::insert(buffer key, buffer value)
{
    result_t result = result_t::success;
    IfFailRet(this->insert(this->m_root, key, value));
    return result;
}

result_t btree_impl::close()
{
    return this->m_caching_layer->close();
}

result_t btree_impl::insert(int node, buffer key, buffer value)
{
    result_t result = result_t::success;
    void* node_memory;
    IfFailRet(this->m_caching_layer->get_page(node, &node_memory));
    node_header* header = (node_header*)node_memory;
    if (header->is_leaf)
    {
        IfFailRet(this->leaf_insert(node_memory, key, value));
    }
    else
    {
        IfFailRet(this->internal_insert(node_memory, key, value));
    }
    return result;
}

result_t btree_impl::leaf_insert(void* node_memory, buffer key, buffer value)
{    
    result_t result = result_t::success;
    node_header* header = (node_header*)node_memory;
    // Step 1: Find the location to perform the insertion

    IfFalseRet(false, result_t::not_implemented);
    return result;
}

result_t btree_impl::internal_insert(void* node_memory, buffer key, buffer value)
{
    result_t result = result_t::success;
    IfFalseRet(false, result_t::not_implemented);
    return result;
}