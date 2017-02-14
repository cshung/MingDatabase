#pragma once

btree::btree(caching_layer* cache)
{
    this->m_impl = new btree_impl(cache);
}

result_t btree::insert(buffer key, buffer value)
{
    return this->m_impl->insert(key, value);
}
