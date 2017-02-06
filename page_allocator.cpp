#include "page_allocator.h"

class page_allocator_impl
{
public:
    page_allocator_impl(page_file* page_file);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
private:
    page_file* m_page_file;
};

#include "page_allocator.forwarders.inl"

page_allocator_impl::page_allocator_impl(page_file* page_file)
{
    this->m_page_file = page_file;
}

result_t page_allocator_impl::read_page(int page_number, void* buffer)
{
    return this->m_page_file->read_page(page_number, buffer);
}

result_t page_allocator_impl::write_page(int page_number, void* buffer)
{
    return this->m_page_file->write_page(page_number, buffer);
}
