#include <halcyon/internal/string.hpp>

#include <halcyon/utility/strutil.hpp>

#include <cstring>

using namespace hal;

sdl::string::string(char* ptr, pass_key<authority_t>)
    : raii_object { ptr }
{
}

std::size_t sdl::string::size() const
{
    return std::strlen(get());
}

char* sdl::string::begin()
{
    return get();
}

char* sdl::string::end()
{
    return get() + size();
}

const char* sdl::string::begin() const
{
    return get();
}

const char* sdl::string::end() const
{
    return get() + size();
}

char* sdl::string::data()
{
    return get();
}

const char* sdl::string::data() const
{
    return get();
}

const char* sdl::string::c_str() const
{
    return get();
}

sdl::string::operator std::string_view() const
{
    return get();
}

bool sdl::operator==(string lhs, std::string_view rhs)
{
    return rhs == lhs.c_str();
}