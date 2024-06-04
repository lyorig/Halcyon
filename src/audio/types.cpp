#include <halcyon/audio/types.hpp>

#include <iostream>

using namespace hal;

std::ostream& audio::operator<<(std::ostream& str, audio::format fmt)
{
    return str << to_string(fmt);
}

std::ostream& audio::operator<<(std::ostream& str, audio::change ch)
{
    return str << to_string(ch);
}