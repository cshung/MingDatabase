#include <iostream>
#include "constant.h"
#include "file_layer.h"
#include "allocator_layer.h"
#include <cstdint>
#include <cstring>

using namespace std;
#pragma warning(disable: 4996)

// #define LOG

result_t run()
{
    uint8_t buffer[PAGE_SIZE];
    result_t result = success;
    remove("hello.db");
    {
        allocator_layer allocator;
        IfFailRet(allocator.open("hello.db"));
        strcpy((char*)buffer, "World");
        int new_page_number;
        IfFailRet(allocator.allocate_page(&new_page_number));
        IfFailRet(allocator.write_page(new_page_number, buffer));
        IfFailRet(allocator.allocate_page(&new_page_number));
        IfFailRet(allocator.deallocate_page(new_page_number));
        IfFailRet(allocator.close());
    }
    {
        allocator_layer allocator;
        IfFailRet(allocator.open("hello.db"));
        IfFailRet(allocator.read_page(1, buffer));
        cout << (char*)buffer << endl;
        IfFailRet(allocator.close());
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
