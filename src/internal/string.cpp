#include <halcyon/internal/string.hpp>

#include <halcyon/utility/strutil.hpp>

#include <cstring>

using namespace hal;

string::string(char* ptr, pass_key<authority_t>)
    : raii_object { ptr }
{
}

std::size_t string::size() const
{
    return std::strlen(get());
}

char* string::begin()
{
    return get();
}

char* string::end()
{
    return get() + size();
}

const char* string::begin() const
{
    return get();
}

const char* string::end() const
{
    return get() + size();
}

char* string::data()
{
    return get();
}

const char* string::data() const
{
    return get();
}

const char* string::c_str() const
{
    return get();
}

string::operator std::string_view() const
{
    return get();
}

bool hal::operator==(string lhs, std::string_view rhs)
{
    return rhs == lhs.c_str();
}