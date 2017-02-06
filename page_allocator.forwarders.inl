page_allocator::page_allocator()
{
    this->m_impl = new page_allocator_impl();
}

result_t page_allocator::open(const char* file_name)
{
    return this->m_impl->open(file_name);
}

result_t page_allocator::read_page(int page_number, void* buffer)
{
    return this->m_impl->read_page(page_number, buffer);
}

result_t page_allocator::write_page(int page_number, void* buffer)
{
    return this->m_impl->write_page(page_number, buffer);
}

result_t page_allocator::close()
{
    return this->m_impl->close();
}

result_t page_allocator::allocate_page(int* new_page_number)
{
    return this->m_impl->allocate_page(new_page_number);
}
