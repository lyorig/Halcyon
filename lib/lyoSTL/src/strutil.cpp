#include <lyo/strutil.hpp>

bool lyo::streq(const char* lhs, const char* rhs) noexcept
{
    return std::strcmp(lhs, rhs) == 0;
}

bool lyo::streq(const wchar_t* lhs, const wchar_t* rhs) noexcept
{
    return std::wcscmp(lhs, rhs) == 0;
}