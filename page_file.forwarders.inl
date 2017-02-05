page_file::page_file()
{
    this->m_impl = new page_file_impl();
}

page_file::~page_file()
{
    delete this->m_impl;
}

result_t page_file::open(const char* file_name)
{
    return this->m_impl->open(file_name);
}

result_t page_file::read_page(int page_number, void* buffer)
{
    return this->m_impl->read_page(page_number, buffer);
}

result_t page_file::write_page(int page_number, void* buffer)
{
    return this->m_impl->write_page(page_number, buffer);
}


result_t page_file::append_page(int* new_page_number)
{
    return this->m_impl->append_page(new_page_number);
}

result_t page_file::close()
{
    return this->m_impl->close();
}