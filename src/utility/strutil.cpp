#include <halcyon/utility/strutil.hpp>

bool hal::streq(const char* lhs, const char* rhs)
{
    return std::strcmp(lhs, rhs) == 0;
}

bool hal::streq(const wchar_t* lhs, const wchar_t* rhs)
{
    return std::wcscmp(lhs, rhs) == 0;
}