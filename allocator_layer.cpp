#include "allocator_layer.h"
#include "constant.h"
#include <vector>
#include <cstdint>
#include <cassert>

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
    virtual result_t on_file_layer_created();
    virtual result_t on_file_layer_loaded();
private:
    result_t save_free_list();
    result_t load_free_list();
    file_layer* m_file_layer;
    vector<uint32_t> m_free_list;
};

#include "allocator_layer.forwarders.inl"

allocator_layer_impl::allocator_layer_impl()
{
    this->m_file_layer = new file_layer();
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

result_t allocator_layer_impl::close()
{
    return this->m_file_layer->close();
}

result_t allocator_layer_impl::on_file_layer_created()
{
    result_t result;
    int new_page_number;
    IfFailRet(this->m_file_layer->append_page(&new_page_number));
    IfFailRet(this->save_free_list());
    return result;
}

result_t allocator_layer_impl::on_file_layer_loaded()
{
    return this->load_free_list();
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