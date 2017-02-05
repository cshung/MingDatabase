#pragma once

#define IfFailRet(x) if ((result = x) != success) { goto error; }

enum result_t
{
    success,
    invalid_argument,
    file_io_error,
};