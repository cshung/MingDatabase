#include "allocator_layer.h"
#include "constant.h"
#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include <stdio.h>

using namespace std;

class allocator_layer_impl : file_layer_listener
{
public:
    allocator_layer_impl();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t close();
    result_t allocate_page(int* new_page_number);
    result_t deallocate_page(int page_number);
    virtual result_t on_after_file_created();
    virtual result_t on_after_file_loaded();
    virtual result_t on_before_file_closed();

    void test();
private:
    bool m_free_list_changed;
    result_t save_free_list();
    result_t load_free_list();
    result_t compact();
    file_layer* m_file_layer;
    vector<uint32_t> m_free_list;
};

#include "allocator_layer.forwarders.inl"

allocator_layer_impl::allocator_layer_impl()
{
    this->m_file_layer = new file_layer();
    this->m_free_list_changed = false;
}

result_t allocator_layer_impl::read_page(int page_number, void* buffer)
{
    return this->m_file_layer->read_page(page_number, buffer);
}

result_t allocator_layer_impl::write_page(int page_number, void* buffer)
{
    return this->m_file_layer->write_page(page_number, buffer);
}

result_t allocator_layer_impl::open(const char* file_name)
{
    result_t result = result_t::success;
    IfFailRet(this->m_file_layer->set_file_layer_listener(this));
    IfFailRet(this->m_file_layer->open(file_name));
    IfFailRet(this->m_file_layer->set_file_layer_listener(nullptr));
    return result;
}

result_t allocator_layer_impl::allocate_page(int* new_page_number)
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
        IfFailRet(this->m_file_layer->append_page(new_page_number));
    }

    return result;
}

result_t allocator_layer_impl::deallocate_page(int page_number)
{
    result_t result = result_t::success;
    this->m_free_list_changed = true;
    this->m_free_list.push_back(page_number);
    if (this->m_free_list.size() > 5)
    {
        IfFailRet(this->compact());
    }

    return result;
}

result_t allocator_layer_impl::close()
{
    result_t result = result_t::success;
    IfFailRet(this->m_file_layer->set_file_layer_listener(this));
    IfFailRet(this->m_file_layer->close());
    IfFailRet(this->m_file_layer->set_file_layer_listener(nullptr));
    return result;
}

result_t allocator_layer_impl::on_after_file_created()
{
    result_t result = result_t::success;
    int new_page_number;
    IfFailRet(this->m_file_layer->append_page(&new_page_number));
    IfFailRet(this->save_free_list());
    return result;
}

result_t allocator_layer_impl::on_after_file_loaded()
{
    return this->load_free_list();
}

result_t allocator_layer_impl::on_before_file_closed()
{
    result_t result = result_t::success;
    if (this->m_free_list_changed)
    {
        IfFailRet(this->save_free_list());
    }
    return result;
}


result_t allocator_layer_impl::save_free_list()
{
    result_t result = result_t::success;
    uint8_t allocation_page[PAGE_SIZE];
    assert(this->m_free_list.size() < 256);
    allocation_page[0] = (uint8_t)this->m_free_list.size();
    for (size_t i = 0; i < this->m_free_list.size(); i++)
    {
        uint32_t free_page = this->m_free_list[i];
        memcpy(allocation_page + 1 + i * 4, &free_page, 4);
    }
    IfFailRet(this->m_file_layer->write_page(0, allocation_page));
    return result;
}

result_t allocator_layer_impl::load_free_list()
{
    result_t result = result_t::success;
    uint8_t allocation_page[PAGE_SIZE];
    IfFailRet(this->m_file_layer->read_page(0, allocation_page));
    int num_free_page = allocation_page[0];
    for (int i = 0; i < num_free_page; i++)
    {
        uint32_t free_page;
        memcpy(&free_page, allocation_page + 1 + i * 4, 4);
        this->m_free_list.push_back(free_page);
    }
    return result;
}

result_t allocator_layer_impl::compact()
{
    result_t result = result_t::success;
    sort(this->m_free_list.begin(), this->m_free_list.end());
    int num_pages;
    IfFailRet(this->m_file_layer->get_num_pages(&num_pages));

    // Debugging
    num_pages = 5;

    // Step 1: Find the first free segment
    int free_list_index = 0;
    int free_start = this->m_free_list[free_list_index++];
    int free_end = free_start + 1;
    while (free_list_index < this->m_free_list.size())
    {
        if (this->m_free_list[free_list_index] != free_end)
        {
            break;
        }
        else
        {
            free_end++;
            free_list_index++;
        }
    }

    // Step 2: Finding the first data segment
    int data_start = free_end;
    int data_end;
    if (free_list_index != this->m_free_list.size())
    {
        data_end = this->m_free_list[free_list_index];
    }
    else
    {
        data_end = num_pages;
    }

    printf("free [%d, %d)\n", free_start, free_end);
    printf("data [%d, %d)\n", data_start, data_end);

    while (true)
    {
        // Inside this loop, we should always have a free segment, but we may not have a data segment
        // That is possible if we have got (or already squeezed) the free segment to the very end of the file
        // If that's the case, we can stop now
        if (data_start == data_end)
        {
            // Ideally, we can reclaim the space in the file as well (i.e. _chsize)
        }
        else
        {
            // TODO: Move the data
        }
    }

    return result;
}

void allocator_layer::test()
{
    this->m_impl->test();
}

void allocator_layer_impl::test()
{
    this->m_free_list.clear();
    this->m_free_list.push_back(1);
    this->m_free_list.push_back(3);
    this->compact();
}