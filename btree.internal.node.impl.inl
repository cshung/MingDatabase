#pragma once

btree_internal_node::btree_internal_node(caching_layer* caching_layer, int page_number) : btree_node(caching_layer, page_number)
{

}

result_t btree_internal_node::insert(int key, int value, insert_result* p_insert_result)
{
    return result_t::not_implemented;
}
