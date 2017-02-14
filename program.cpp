#include <iostream>
#include "constant.h"
#include "file_layer.h"
#include "allocator_layer.h"
#include "caching_layer.h"
#include <cstdint>
#include <cstring>

using namespace std;
#pragma warning(disable: 4996)

// #define LOG

result_t run()
{
    void* buffer;
    result_t result = success;
    remove("hello.db");
    {
        caching_layer caching_layer;
        IfFailRet(caching_layer.open("hello.db"));
        int new_page_number;
        IfFailRet(caching_layer.allocate_page(&new_page_number));
        IfFailRet(caching_layer.get_page(new_page_number, &buffer));
        strcpy((char*)buffer, "World");
        IfFailRet(caching_layer.set_page_as_written(new_page_number));
        IfFailRet(caching_layer.allocate_page(&new_page_number));
        IfFailRet(caching_layer.deallocate_page(new_page_number));
        IfFailRet(caching_layer.close());
    }
    {
        caching_layer caching_layer;
        IfFailRet(caching_layer.open("hello.db"));
        IfFailRet(caching_layer.get_page(1, &buffer));
        cout << (char*)buffer << endl;
        IfFailRet(caching_layer.close());
    }

    return result;
}

int main(int argc, char** argv)
{
    result_t result = run();

    if (result != result_t::success)
    {
        cout << "Ooops, an error occurred: " << result << endl;
    }

    return 0;
}
