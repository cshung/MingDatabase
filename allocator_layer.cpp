#include "allocator_layer.h"
#include "file_layer.h"
#include "constant.h"
#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <string.h>

#include <stdio.h>

using namespace std;

class allocator_layer_impl : file_layer_listener
{
public:
    allocator_layer_impl();
    ~allocator_layer_impl();
    result_t open(const char* file_name);
    result_t read_page(int page_number, void* buffer);
    result_t write_page(int page_number, void* buffer);
    result_t get_root(int* root);
    result_t set_root(int root);
    result_t allocate_page(int* new_page_number);
    result_t deallocate_page(int page_number);
    result_t close();
    virtual result_t on_after_file_created();
    virtual result_t on_after_file_loaded();
    virtual result_t on_before_file_closed();
private:
    bool m_free_list_changed;
    result_t save_free_list();
    result_t load_free_list();
    result_t compact();
    file_layer* m_file_layer;
    uint32_t m_root;
    vector<uint32_t> m_free_list;
};

#include "allocator_layer.forwarders.inl"

allocator_layer_impl::allocator_layer_impl()
{
    this->m_file_layer = new file_layer();
    this->m_root = 0;
    this->m_free_list_changed = false;
}

allocator_layer_impl::~allocator_layer_impl()
{
    delete this->m_file_layer;
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

result_t allocator_layer_impl::get_root(int* root)
{
    result_t result = result_t::success;
    IfFalseRet(root != nullptr, result_t::invalid_argument);
    *root = this->m_root;
    return result;
}

result_t allocator_layer_impl::set_root(int root)
{
    result_t result = result_t::success;
    this->m_root = root;
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
        this->m_free_list_changed = true;
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
    if (this->m_free_list.size() >= 5)
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
    uint32_t allocation_page[PAGE_SIZE/sizeof(uint32_t)];
    assert(this->m_free_list.size() < 256);
    allocation_page[0] = this->m_root;
    allocation_page[1] = this->m_free_list.size();
    for (size_t i = 0; i < this->m_free_list.size(); i++)
    {
        allocation_page[i + 2] = this->m_free_list[i];        
    }
    IfFailRet(this->m_file_layer->write_page(0, allocation_page));
    return result;
}

result_t allocator_layer_impl::load_free_list()
{
    result_t result = result_t::success;
    uint32_t allocation_page[PAGE_SIZE / sizeof(uint32_t)];
    IfFailRet(this->m_file_layer->read_page(0, allocation_page));
    this->m_root = allocation_page[0];
    int num_free_page = allocation_page[1];
    for (int i = 0; i < num_free_page; i++)
    {
        this->m_free_list.push_back(allocation_page[i + 2]);
    }
    return result;
}

result_t allocator_layer_impl::compact()
{
    result_t result = result_t::success;
    assert(this->m_free_list.size() > 0);

    sort(this->m_free_list.begin(), this->m_free_list.end());
    int num_pages;
    IfFailRet(this->m_file_layer->get_num_pages(&num_pages));

    // The end state of the file should have [1, data_end) pages
    size_t data_end = num_pages - this->m_free_list.size();

    // To do so, we want to walk two pointers and fill the holes
    // On one hand, we want to walk the free slots before data_end (this is free_list_index_1)
    // On the other hand, we want to walk the data slots on or after data_end (this is data_slot)
    // The problem is that we do not know which slot after data_end is free, so we walk the free list
    // until we reach after data_end and skip them as we see them. 
    // The upcoming free slot after data_end is stored as free_list_index_2

    size_t free_list_index_1 = 0;
    size_t data_slot = data_end;
    size_t free_list_index_2 = 0;
    while ((free_list_index_2 < this->m_free_list.size()) && (this->m_free_list[free_list_index_2] < data_end))
    {
        free_list_index_2++;
    }

    while ((free_list_index_1 < this->m_free_list.size()) && (this->m_free_list[free_list_index_1] < data_end))
    {
        while ((free_list_index_2 < this->m_free_list.size()) && (this->m_free_list[free_list_index_2] == data_slot))
        {
            free_list_index_2++;
            data_slot++;
        }

        // TODO, is it possible to postpone the file I/O operation required for compacting?
        // TODO, if the page number is referenced, we need to make sure the reference is updated as well.
        // In particular, we need to make sure the caching_layer is updated so that it won't get the wrong
        // page cached.
        uint8_t buffer[PAGE_SIZE];
        IfFailRet(this->m_file_layer->read_page((int)data_slot, buffer));
        IfFailRet(this->m_file_layer->write_page((int)this->m_free_list[free_list_index_1], buffer));

        free_list_index_1++;
        data_slot++;
    }

    assert(data_slot == num_pages);
    this->m_free_list.clear();

    // TODO - shrink the file - so that we actually compact
    // Unfortunately, it looks like actually truncate a file has to be platform specific ??
    for (size_t i = data_end; i < num_pages; i++)
    {
        this->m_free_list.push_back((int)i);
    }

    return result;
}
