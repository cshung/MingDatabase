#include "btree.h"
#include "constant.h"

class btree_impl
{
public:
    btree_impl(caching_layer* caching_layer, comparator* comparator, int root);
    result_t initialize();
    result_t insert(buffer key, buffer value);
private:
    result_t insert(int node, buffer key, buffer value);
    result_t leaf_insert(uint8_t* node_memory, buffer key, buffer value);
    result_t internal_insert(uint8_t* node_memory, buffer key, buffer value);
    result_t get_key(uint8_t* node_memory, int index, buffer* key);
    caching_layer* m_caching_layer;
    comparator* m_comparator;
    int m_root;
};

#include "btree.forwarders.inl"

// The layout of a node page is as follow:
// It starts with a node_header that tell if it is a leaf node or an internal node.
// For a leaf node, we need to store the keys and the values.
// They are packed sequentially with a 2 byte length prefix following the payload immediately
// This layout does not allow random access, and therefore we have another index.
// For each key, it's offset from the beginning of the page is stored as an array.
// The array started at the end of the page and run backwards.

struct node_header
{
    bool is_leaf;
    int num_keys;
};

btree_impl::btree_impl(caching_layer* caching_layer, comparator* comparator, int root)
{
    assert(caching_layer != nullptr);
    assert(comparator != nullptr);
    this->m_caching_layer = caching_layer;
    this->m_comparator = comparator;
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
    return result;
}

result_t btree_impl::insert(buffer key, buffer value)
{
    result_t result = result_t::success;
    IfFailRet(this->insert(this->m_root, key, value));
    return result;
}

result_t btree_impl::insert(int node, buffer key, buffer value)
{
    result_t result = result_t::success;
    uint8_t* node_memory;
    IfFailRet(this->m_caching_layer->get_page(node, (void**)&node_memory));
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

result_t btree_impl::leaf_insert(uint8_t* node_memory, buffer key, buffer value)
{    
    result_t result = result_t::success;
    node_header* header = (node_header*)node_memory;
    // Step 1: Find the location to perform the insertion
    for (int i = 0; i < header->num_keys; i++)
    {

    }

    IfFalseRet(false, result_t::not_implemented);
    return result;
}

result_t btree_impl::internal_insert(uint8_t* node_memory, buffer key, buffer value)
{
    result_t result = result_t::success;
    IfFalseRet(false, result_t::not_implemented);
    return result;
}

result_t btree_impl::get_key(uint8_t* node_memory, int index, buffer* key)
{
    result_t result = result_t::success;
    IfFalseRet(key != nullptr, result_t::invalid_argument);
    node_header* header = (node_header*)node_memory;
    IfFalseRet(index >= 0, result_t::invalid_argument);
    IfFalseRet(index < header->num_keys, result_t::invalid_argument);
    uint16_t* last = (uint16_t*)(node_memory + PAGE_SIZE);
    uint16_t offset = *(last - index - 1);
    assert(offset < PAGE_SIZE);
    uint16_t* buffer = (uint16_t*)(node_memory + offset);
    key->size = *buffer;
    key->data = (void*)(buffer + 1);
    return result;
}