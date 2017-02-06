#include "page_allocator.h"

class page_allocator_impl : page_file_creation_listener
{
public:
    page_allocator_impl();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t close();
    virtual result_t on_page_file_created();
private:
    page_file* m_page_file;
};

#include "page_allocator.forwarders.inl"

page_allocator_impl::page_allocator_impl()
{
    this->m_page_file = new page_file();
}

result_t page_allocator_impl::read_page(int page_number, void* buffer)
{
    return this->m_page_file->read_page(page_number, buffer);
}

result_t page_allocator_impl::write_page(int page_number, void* buffer)
{
    return this->m_page_file->write_page(page_number, buffer);
}

result_t page_allocator_impl::open(const char* file_name)
{
    result_t result = result_t::success;
    IfFailRet(this->m_page_file->set_page_file_creation_listener(this));
    IfFailRet(this->m_page_file->open(file_name));
    IfFailRet(this->m_page_file->set_page_file_creation_listener(nullptr));
    return result;
}

result_t page_allocator_impl::close()
{
    return this->m_page_file->close();
}

result_t page_allocator_impl::on_page_file_created()
{
    result_t result;
    int new_page_number;
    IfFailRet(this->m_page_file->append_page(&new_page_number));
    return result;
}