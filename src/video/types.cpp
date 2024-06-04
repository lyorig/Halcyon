#include <halcyon/video/types.hpp>

using namespace hal;

std::ostream& hal::operator<<(std::ostream& str, blend_mode bm)
{
    return str << to_string(bm);
}

std::ostream& pixel::operator<<(std::ostream& str, pixel::format fmt)
{
    return str << to_string(fmt);
}

std::ostream& pixel::operator<<(std::ostream& str, pixel::type tp)
{
    return str << to_string(tp);
}