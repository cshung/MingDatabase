#include "page_file.h"
#include "constant.h"
#include <cstdio>
#include <cstdint>

// Stop bugging me with the secure versions of the APIs
#pragma warning(disable: 4996)

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
    result_t set_page_file_listener(page_file_listener* page_file_listener);
private:
    FILE* m_file;
    int m_num_pages;
    page_file_listener* m_page_file_listener;
};

#include "page_file.forwarders.inl"

page_file_impl::page_file_impl()
{
    this->m_file = nullptr;
    this->m_page_file_listener = nullptr;
}

page_file_impl::~page_file_impl()
{
}

result_t page_file_impl::open(const char* file_name)
{
    result_t result = result_t::success;
    this->m_file = fopen(file_name, "rb+");
    if (this->m_file == nullptr)
    {
        this->m_file = fopen(file_name, "wb+");
        if (this->m_file == nullptr)
        {
            return result_t::file_io_error;
        }

        this->m_num_pages = 0;
        if (this->m_page_file_listener != nullptr)
        {
            IfFailRet(this->m_page_file_listener->on_page_file_created());
        }
    }
    else
    {
        if (fseek(this->m_file, 0, SEEK_END) == -1)
        {
            return result_t::file_io_error;
        }

        this->m_num_pages = ftell(this->m_file) / PAGE_SIZE;
        if (this->m_page_file_listener != nullptr)
        {
            IfFailRet(this->m_page_file_listener->on_page_file_loaded());
        }
    }
    return result;
}

result_t page_file_impl::close()
{
    if (fclose(this->m_file) != 0)
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

    if (fseek(this->m_file, page_number * PAGE_SIZE, SEEK_SET) == -1)
    {
        return result_t::file_io_error;
    }

    if (fread(buffer, PAGE_SIZE, 1, this->m_file) != 1)
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

    if (fseek(this->m_file, page_number * PAGE_SIZE, SEEK_SET) == -1)
    {
        return result_t::file_io_error;
    }

    if (fwrite(buffer, PAGE_SIZE, 1, this->m_file) != 1)
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
    if (fseek(this->m_file, 0, SEEK_END) == -1)
    {
        return result_t::file_io_error;
    }

    if (fwrite(blank_page, PAGE_SIZE, 1, this->m_file) != 1)
    {
        return result_t::file_io_error;
    }

    *new_page_size = this->m_num_pages++;
    return result_t::success;
}

result_t page_file_impl::set_page_file_listener(page_file_listener* page_file_listener)
{
    this->m_page_file_listener = page_file_listener;
    return result_t::success;
}
