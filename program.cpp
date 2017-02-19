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
