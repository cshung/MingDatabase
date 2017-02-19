#pragma once

struct insert_result
{
    bool succeed;
    bool overflow;
};

class btree_node
{
public:
    btree_node(caching_layer* caching_layer, int page_number);
    static result_t create_btree_node(caching_layer* caching_layer, int page_number, btree_node** ppNode);
    virtual result_t insert(int key, int value, insert_result* p_insert_result) = 0;
private:
    caching_layer* m_caching_layer;
    int m_page_number;
};

class btree_internal_node : public btree_node
{
public:
    btree_internal_node(caching_layer* caching_layer, int page_number);
    virtual result_t insert(int key, int value, insert_result* p_insert_result);
};

class btree_leaf_node : public btree_node
{
public:
    btree_leaf_node(caching_layer* caching_layer, int page_number);
    virtual result_t insert(int key, int value, insert_result* p_insert_result);
};
