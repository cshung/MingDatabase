#include "page_file.h"
#include "constant.h"
#include <cstdio>
#include <errno.h>
#include <cstdint>

class page_file_impl
{
public:
    page_file_impl();
    ~page_file_impl();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t append_page(int* new_page_number);
    result_t close();
private:
    FILE* m_file;
    int m_num_pages;
};

#include "page_file.forwarders.inl"

page_file_impl::page_file_impl()
{
}

page_file_impl::~page_file_impl()
{
}

result_t page_file_impl::open(const char* file_name)
{
    this->m_file = fopen(file_name, "rb+");
    if (this->m_file == nullptr)
    {
        this->m_file = fopen(file_name, "wb+");
        if (errno != 0)
        {
            return result_t::file_io_error;
        }
    }
    fseek(this->m_file, 0, SEEK_END);
    if (errno != 0)
    {
        return result_t::file_io_error;
    }
    this->m_num_pages = ftell(this->m_file);
    return result_t::success;
}

result_t page_file_impl::close()
{
    fclose(this->m_file);
    if (errno != 0)
    {
        return result_t::file_io_error;
    }
    this->m_file = nullptr;
    return result_t::success;
}

result_t page_file_impl::read_page(int page_number, void* buffer)
{
    if (page_number >= this->m_num_pages)
    {
        return result_t::file_io_error;
    }

    fseek(this->m_file, page_number * PAGE_SIZE, SEEK_SET);
    if (errno != 0)
    {
        return result_t::file_io_error;
    }
    fread(buffer, PAGE_SIZE, 1, this->m_file);
    if (errno != 0)
    {
        return result_t::file_io_error;
    }
    return result_t::success;
}

result_t page_file_impl::write_page(int page_number, void* buffer)
{
    if (page_number >= this->m_num_pages)
    {
        return result_t::file_io_error;
    }

    fseek(this->m_file, page_number * PAGE_SIZE, SEEK_SET);
    if (errno != 0)
    {
        return result_t::file_io_error;
    }
    fwrite(buffer, PAGE_SIZE, 1, this->m_file);
    if (errno != 0)
    {
        return result_t::file_io_error;
    }
    return result_t::success;
}

result_t page_file_impl::append_page(int* new_page_size)
{
    if (new_page_size == nullptr)
    {
        return result_t::invalid_argument;
    }

    uint8_t blank_page[PAGE_SIZE];
    fseek(this->m_file, 0, SEEK_END);
    if (errno != 0)
    {
        return result_t::file_io_error;
    }
    fwrite(blank_page, PAGE_SIZE, 1, this->m_file);
    if (errno != 0)
    {
        return result_t::file_io_error;
    }
    *new_page_size = this->m_num_pages++;
    return result_t::success;
}
