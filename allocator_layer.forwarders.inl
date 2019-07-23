allocator_layer::allocator_layer()
{
    this->m_impl = new allocator_layer_impl();
}

allocator_layer::~allocator_layer()
{
    delete this->m_impl;
}

result_t allocator_layer::open(const char* file_name)
{
    return this->m_impl->open(file_name);
}

result_t allocator_layer::read_page(int page_number, void* buffer)
{
    return this->m_impl->read_page(page_number, buffer);
}

result_t allocator_layer::write_page(int page_number, void* buffer)
{
    return this->m_impl->write_page(page_number, buffer);
}

result_t allocator_layer::close()
{
    return this->m_impl->close();
}

result_t allocator_layer::allocate_page(int* new_page_number)
{
    return this->m_impl->allocate_page(new_page_number);
}

result_t allocator_layer::deallocate_page(int page_number)
{
    return this->m_impl->deallocate_page(page_number);
}