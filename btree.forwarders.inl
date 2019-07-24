#pragma once

btree::btree(caching_layer* caching_layer, int root)
{
    this->m_impl = new btree_impl(caching_layer, root);
}

result_t btree::initialize()
{
    return this->m_impl->initialize();
}

result_t btree::insert(buffer key, buffer value)
{
    return this->m_impl->insert(key, value);
}


result_t btree::close()
{
    return this->m_impl->close();
}