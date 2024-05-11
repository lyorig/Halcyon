#include <halcyon/internal/string.hpp>

#include <halcyon/utility/strutil.hpp>

using namespace hal::sdl;

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

bool hal::sdl::operator==(string lhs, std::string_view rhs)
{
    return hal::streq(lhs.c_str(), rhs.data());
}