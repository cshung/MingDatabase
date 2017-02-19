#pragma once

btree_node::btree_node(caching_layer* caching_layer, int page_number)
{
    this->m_caching_layer = caching_layer;
    this->m_page_number = page_number;
}

result_t btree_node::create_btree_node(caching_layer* caching_layer, int page_number, btree_node** ppNode)
{
    result_t result = result_t::success;
    void* page;
    IfFailRet(caching_layer->get_page(page_number, &page));
    page_header* header = (page_header*)page;
    if (header->is_leaf)
    {
        *ppNode = new btree_leaf_node(caching_layer, page_number);
    }
    else
    {
        *ppNode = new btree_internal_node(caching_layer, page_number);
    }

    return result;
}
