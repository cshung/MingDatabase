file_layer::file_layer()
{
    this->m_impl = new file_layer_impl();
}

file_layer::~file_layer()
{
    delete this->m_impl;
}

result_t file_layer::open(const char* file_name)
{
    return this->m_impl->open(file_name);
}

result_t file_layer::read_page(int page_number, void* buffer)
{
    return this->m_impl->read_page(page_number, buffer);
}

result_t file_layer::write_page(int page_number, void* buffer)
{
    return this->m_impl->write_page(page_number, buffer);
}


result_t file_layer::append_page(int* new_page_number)
{
    return this->m_impl->append_page(new_page_number);
}

result_t file_layer::close()
{
    return this->m_impl->close();
}

result_t file_layer::set_file_layer_listener(file_layer_listener* file_layer_listener)
{
    return this->m_impl->set_file_layer_listener(file_layer_listener);
}
