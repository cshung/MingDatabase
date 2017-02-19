#include <iostream>
#include "constant.h"
#include "file_layer.h"
#include "allocator_layer.h"
#include "caching_layer.h"
#include "btree.h"
#include <cstdint>
#include <cstring>

using namespace std;
#pragma warning(disable: 4996)

// #define LOG

result_t run()
{
    result_t result = success;
    remove("hello.db");
    {
        caching_layer caching_layer;
        IfFailRet(caching_layer.open("hello.db"));
        btree tree(&caching_layer);

        const char* key = "hello";
        const char* val = "world";
        buffer key_buffer;
        key_buffer.data = (void*)key;
        key_buffer.size = 5;

        buffer val_buffer;
        val_buffer.data = (void*)val;
        val_buffer.size = 5;

        tree.insert(key_buffer, val_buffer);

        caching_layer.close();
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
