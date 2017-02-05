page_allocator::page_allocator(page_file* page_file)
{
    this->m_impl = new page_allocator_impl(page_file);
}

result_t page_allocator::read_page(int page_number, void* buffer)
{
    return this->m_impl->read_page(page_number, buffer);
}
