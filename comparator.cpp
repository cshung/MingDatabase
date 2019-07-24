#include "comparator.h"
#include <algorithm>
#include <string.h>

result_t string_comparator::compare(buffer key1, buffer key2, int* answer)
{
    result_t result = result_t::success;
    IfFalseRet(answer != nullptr, result_t::invalid_argument);
    uint16_t length = std::min(key1.size, key2.size);
    int compare = strncmp((char*)key1.data, (char*)key2.data, length);
    if (compare == 0)
    {
        *answer = key1.size - key2.size;        
    }
    else
    {
        *answer = compare;
    }
    return result;
}