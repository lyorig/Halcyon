#include <halcyon/types/color.hpp>
#include <halcyon/utility/printing.hpp>
#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>

using namespace hal;

// The linker takes issue with this not being explicitly namespaced.
std::ostream& hal::operator<<(std::ostream& str, color c)
{
    return str << "[r" << to_printable_int(c.r) << " g" << to_printable_int(c.g) << " b" << to_printable_int(c.b) << " a" << to_printable_int(c.a) << ']';
}

std::ostream& hal::operator<<(std::ostream& str, const info::display& disp)
{
    return str << "[size: " << disp.size << ", format: " << to_string(disp.fmt) << ", Hz: " << disp.hz << ']';
}

std::ostream& hal::operator<<(std::ostream& str, const info::renderer& inf)
{
    return str << '[' << inf.name() << ", flags: 0x" << std::hex << inf.flags().mask() << std::dec << ", MTS: " << inf.max_texture_size() << ']';
}