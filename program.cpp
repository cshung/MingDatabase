#include <iostream>
#include "constant.h"
#include "page_file.h"
#include "page_allocator.h"
#include <cstdint>
#include <cstring>

using namespace std;
#pragma warning(disable: 4996)

// #define LOG

int main(int argc, char** argv)
{
    uint8_t buffer[PAGE_SIZE];
    bool red = true;
    {
        page_file file;
        file.open("hello.db");
        strcpy((char*)buffer, "World");
        int new_page_number = 0;
        file.append_page(&new_page_number);
        file.write_page(0, buffer);
        file.close();
    }
    {
        page_allocator allocator;
        allocator.open("hello.db");
        allocator.read_page(0, buffer);
        cout << (char*)buffer << endl;
        allocator.close();
    }

    return 0;
}
