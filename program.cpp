#include <iostream>
#include "constant.h"
#include "page_file.h"
#include "page_allocator.h"
#include <cstdint>

using namespace std;

// #define LOG

int main(int argc, char** argv)
{
    page_file file;
    file.open("hello.db");
    uint8_t buffer[PAGE_SIZE];
    bool red = true;
    if (red)
    {
        page_allocator allocator(&file);
        result_t result = allocator.read_page(0, buffer);
        cout << result;
        cout << (char*)buffer << endl;
    }
    else
    {
        strcpy((char*)buffer, "World");
        int new_page_number = 0;
        file.append_page(&new_page_number);
        file.write_page(0, buffer);
    }
    file.close();
    return 0;
}
