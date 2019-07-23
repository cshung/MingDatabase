#pragma once

#include <cassert>

//
// A few words on error handling
//
// In general, we want to fail fast. The program should stop whenever there is an error.
// Instead of doing through exception - which is costly and less portable - we do it through returning error code
// The error code is on the stack, there is no global state (e.g. errno) or thread local state (e.g. GetLastError())
//
// Often, return code is hard to diagnose, to this end, we assert on unexpected failure.
// Most, if not all, operations involves file I/O, and file I/O errors are uncontrollable, we let them through.
//
// By convention, all the methods (except the constructors and destructors) return result_t. This is done for two
// reasons.
//
// 1.) It is consistent across - certain method you know it will not have an error condition (e.g. set callback)
//     but it is much easier to have a consistent model everywhere.
//
// 2.) It allows for changes - adding the return type when it has to change would be difficult, especially so when
//     callers accumulates, and caller's caller, and so on.
//

#define IfFailRet(x) if ((result = (x)) != success) { assert(result == result_t::file_io_error); return result; }
#define IfFalseRet(x, result) if (!(x)) { IfFailRet(result); }

enum result_t
{
    success,
    invalid_argument,
    file_io_error,
    not_implemented
};