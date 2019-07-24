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
        // A unit test for the compaction algorithm
        // Page 1 - 10 is allocated, page 1,3,5,7,9 are deallocated,
        // The free list accumulates to 5 pages and start compacting
        // It results in page 6 copied into page 1
        // It results in page 8 copied into page 3
        // It results in page 10 copied into page 5
        // And we are done
        allocator_layer allocator_layer;
        int page_number;
        IfFailRet(allocator_layer.open("hello.db"));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.allocate_page(&page_number));
        IfFailRet(allocator_layer.deallocate_page(1));
        IfFailRet(allocator_layer.deallocate_page(3));
        IfFailRet(allocator_layer.deallocate_page(5));
        IfFailRet(allocator_layer.deallocate_page(7));
        IfFailRet(allocator_layer.deallocate_page(9));
        IfFailRet(allocator_layer.close());
    }
    {
        allocator_layer allocator_layer;
        IfFailRet(allocator_layer.open("hello.db"));
        IfFailRet(allocator_layer.close());
    }
    remove("hello.db");
    {
        caching_layer caching_layer;
        IfFailRet(caching_layer.open("hello.db"));
        int root;
        IfFailRet(caching_layer.get_root(&root));
        string_comparator comp;
        btree tree(&caching_layer, &comp, root);
        IfFailRet(tree.initialize());

        const char* key = "hello";
        const char* val = "world";
        buffer key_buffer;
        key_buffer.data = (void*)key;
        key_buffer.size = 5;

        buffer val_buffer;
        val_buffer.data = (void*)val;
        val_buffer.size = 5;

        IfFailRet(tree.insert(key_buffer, val_buffer));
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
