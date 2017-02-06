#include <iostream>
#include "constant.h"
#include "page_file.h"
#include "page_allocator.h"
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
        /*
        page_file file;
        IfFailRet(file.open("hello.db"));
        strcpy((char*)buffer, "World");
        int new_page_number = 0;
        IfFailRet(file.append_page(&new_page_number));
        IfFailRet(file.write_page(0, buffer));
        IfFailRet(file.close());
        */
        page_allocator allocator;
        IfFailRet(allocator.open("hello.db"));
        strcpy((char*)buffer, "World");
        IfFailRet(allocator.write_page(0, buffer));
        IfFailRet(allocator.close());
    }
    {
        page_allocator allocator;
        IfFailRet(allocator.open("hello.db"));
        IfFailRet(allocator.read_page(0, buffer));
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
