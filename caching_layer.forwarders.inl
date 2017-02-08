caching_layer::caching_layer()
{
    this->m_impl = new caching_layer_impl();
}

caching_layer::~caching_layer()
{
    delete this->m_impl;
}

result_t caching_layer::open(const char* file_name)
{
    return this->m_impl->open(file_name);
}

result_t caching_layer::get_page(int page_number, void** buffer)
{
    return this->m_impl->get_page(page_number, buffer);
}

result_t caching_layer::put_page(int page_number, void** buffer, bool is_written)
{
    return this->m_impl->put_page(page_number, buffer, is_written);
}

result_t caching_layer::allocate_page(int* new_page_number)
{
    return this->m_impl->allocate_page(new_page_number);
}

result_t caching_layer::deallocate_page(int page_number)
{
    return this->m_impl->deallocate_page(page_number);
}

result_t caching_layer::close()
{
    return this->m_impl->close();
}

