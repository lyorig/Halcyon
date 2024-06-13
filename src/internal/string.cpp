#include <halcyon/internal/string.hpp>

#include <halcyon/utility/strutil.hpp>

#include <cstring>

using namespace hal;

using sv = view::string;

std::size_t sv::size() const
{
    return std::strlen(get());
}

const char* sv::begin() const
{
    return get();
}

const char* sv::end() const
{
    return get() + size();
}

const char* sv::data() const
{
    return get();
}

const char* sv::c_str() const
{
    return get();
}

sv::operator std::string_view() const
{
    return get();
}

string::string(char* ptr, pass_key<authority_t>)
    : raii_object { ptr }
{
}

char* string::begin()
{
    return get();
}

char* string::end()
{
    return get() + size();
}

char* string::data()
{
    return get();
}

bool hal::operator==(string lhs, std::string_view rhs)
{
    return rhs == lhs.c_str();
}