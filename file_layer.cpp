#include "file_layer.h"
#include "constant.h"
#include <cstdio>
#include <cstdint>

// Stop bugging me with the secure versions of the APIs
#pragma warning(disable: 4996)

class file_layer_impl
{
public:
    file_layer_impl();
    ~file_layer_impl();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t append_page(int* new_page_number);
    result_t close();
    result_t set_file_layer_listener(file_layer_listener* file_layer_listener);
    result_t get_num_pages(int* num_pages);
private:
    FILE* m_file;
    int m_num_pages;
    file_layer_listener* m_file_layer_listener;
};

#include "file_layer.forwarders.inl"

file_layer_impl::file_layer_impl()
{
    this->m_file = nullptr;
    this->m_file_layer_listener = nullptr;
}

file_layer_impl::~file_layer_impl()
{
}

result_t file_layer_impl::open(const char* file_name)
{
    result_t result = result_t::success;
    IfFalseRet(file_name != nullptr, result_t::invalid_argument);
    this->m_file = fopen(file_name, "rb+");
    if (this->m_file == nullptr)
    {
        this->m_file = fopen(file_name, "wb+");
        if (this->m_file == nullptr)
        {
            return result_t::file_io_error;
        }

        this->m_num_pages = 0;
        if (this->m_file_layer_listener != nullptr)
        {
            IfFailRet(this->m_file_layer_listener->on_after_file_created());
        }
    }
    else
    {
        if (fseek(this->m_file, 0, SEEK_END) == -1)
        {
            return result_t::file_io_error;
        }

        this->m_num_pages = ftell(this->m_file) / PAGE_SIZE;
        if (this->m_file_layer_listener != nullptr)
        {
            IfFailRet(this->m_file_layer_listener->on_after_file_loaded());
        }
    }
    return result;
}

result_t file_layer_impl::close()
{
    result_t result = result_t::success;
    if (this->m_file_layer_listener != nullptr)
    {
        IfFailRet(this->m_file_layer_listener->on_before_file_closed());
    }
    if (fclose(this->m_file) != 0)
    {
        return result_t::file_io_error;
    }
    this->m_file = nullptr;
    return result;
}

result_t file_layer_impl::read_page(int page_number, void* buffer)
{
    result_t result = result_t::success;

    IfFalseRet(page_number >= 0, result_t::invalid_argument);
    IfFalseRet(page_number < this->m_num_pages, result_t::invalid_argument);
    IfFalseRet(buffer != nullptr, result_t::invalid_argument);

    if (fseek(this->m_file, page_number * PAGE_SIZE, SEEK_SET) == -1)
    {
        return result_t::file_io_error;
    }

    if (fread(buffer, PAGE_SIZE, 1, this->m_file) != 1)
    {
        return result_t::file_io_error;
    }

    return result;
}

result_t file_layer_impl::write_page(int page_number, void* buffer)
{
    result_t result = result_t::success;

    IfFalseRet(page_number >= 0, result_t::invalid_argument);
    IfFalseRet(page_number < this->m_num_pages, result_t::invalid_argument);
    IfFalseRet(buffer != nullptr, result_t::invalid_argument);

    if (fseek(this->m_file, page_number * PAGE_SIZE, SEEK_SET) == -1)
    {
        return result_t::file_io_error;
    }

    if (fwrite(buffer, PAGE_SIZE, 1, this->m_file) != 1)
    {
        return result_t::file_io_error;
    }

    return result;
}

result_t file_layer_impl::append_page(int* new_page_number)
{
    result_t result = result_t::success;
    IfFalseRet(new_page_number != nullptr, result_t::invalid_argument);

    uint8_t blank_page[PAGE_SIZE];
    if (fseek(this->m_file, 0, SEEK_END) == -1)
    {
        return result_t::file_io_error;
    }

    if (fwrite(blank_page, PAGE_SIZE, 1, this->m_file) != 1)
    {
        return result_t::file_io_error;
    }

    *new_page_number = this->m_num_pages++;
    return result;
}

result_t file_layer_impl::set_file_layer_listener(file_layer_listener* file_layer_listener)
{
    this->m_file_layer_listener = file_layer_listener;
    return result_t::success;
}

result_t file_layer_impl::get_num_pages(int* num_pages)
{
    result_t result = result_t::success;
    IfFalseRet(num_pages != nullptr, result_t::invalid_argument);
    *num_pages = this->m_num_pages;
    return result;
}