#pragma once

btree_leaf_node::btree_leaf_node(caching_layer* caching_layer, int page_number) : btree_node(caching_layer, page_number)
{

}

result_t btree_leaf_node::insert(buffer key, buffer value, insert_result* p_insert_result)
{
    return result_t::not_implemented;
}
