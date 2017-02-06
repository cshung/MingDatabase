#include "page_allocator.h"
#include "constant.h"
#include <vector>
#include <cstdint>
#include <cassert>

using namespace std;

class page_allocator_impl : page_file_creation_listener
{
public:
    page_allocator_impl();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t close();
    result_t allocate_page(int* new_page_number);
    virtual result_t on_page_file_created();
private:
    result_t save_allocation();

    page_file* m_page_file;
    vector<int> m_free_list;
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
    IfFailRet(this->save_allocation());
    return result;
}

result_t page_allocator_impl::save_allocation()
{
    result_t result = result_t::success;
    uint8_t allocation_page[PAGE_SIZE];
    for (int i = 0; i < PAGE_SIZE; i++)
    {
        allocation_page[i] = 0xcd;
    }
    assert(this->m_free_list.size() < 256);
    allocation_page[0] = (uint8_t)this->m_free_list.size();
    for (size_t i = 0; i < this->m_free_list.size(); i++)
    {
        int free_page = this->m_free_list[i];
        memcpy(allocation_page + 1 + i * 4, &free_page, 4);
    }
    IfFailRet(this->m_page_file->write_page(0, allocation_page));
    return result;
}

result_t page_allocator_impl::allocate_page(int* new_page_number)
{
    result_t result = result_t::success;
    if (new_page_number == nullptr)
    {
        return result_t::invalid_argument;
    }

    if (this->m_free_list.size() > 0)
    {
        *new_page_number = this->m_free_list.back();
        this->m_free_list.pop_back();
    }
    else
    {
        IfFailRet(this->m_page_file->append_page(new_page_number));
    }

    return result;
}