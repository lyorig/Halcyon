#include <halcyon/internal/string.hpp>

#include <halcyon/utility/strutil.hpp>

#include <cstring>

using namespace hal;

// Const string view.

using cv = view<const string>;

std::size_t cv::size() const
{
    return std::strlen(get());
}

const char* cv::begin() const
{
    return get();
}

const char* cv::end() const
{
    return get() + size();
}

const char* cv::data() const
{
    return get();
}

const char* cv::c_str() const
{
    return get();
}

cv::operator std::string_view() const
{
    return get();
}

// String view.

using v = view<string>;

char* v::begin()
{
    return get();
}

char* v::end()
{
    return get() + size();
}

char* v::data()
{
    return get();
}

// RAII string.

string::string(char* ptr, pass_key<authority_t>)
    : raii_object { ptr }
{
}

bool hal::operator==(string lhs, std::string_view rhs)
{
    return rhs == lhs.c_str();
}