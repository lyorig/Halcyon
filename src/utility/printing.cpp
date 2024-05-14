#include <halcyon/types/color.hpp>
#include <halcyon/utility/printing.hpp>
#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>

using namespace hal;

// The linker takes issue with this not being explicitly namespaced.
std::ostream& hal::operator<<(std::ostream& str, const display& disp)
{
    return str << '[' << disp.name() << ", index " << hal::to_printable_int(disp.index()) << ", size " << disp.size() << ", " << disp.hz() << " Hz]";
}

std::ostream& hal::operator<<(std::ostream& str, color c)
{
    return str << "[r" << to_printable_int(c.r) << " g" << to_printable_int(c.g) << " b" << to_printable_int(c.b) << " a" << to_printable_int(c.a) << ']';
}

std::ostream& hal::operator<<(std::ostream& str, const info::renderer& inf)
{
    return str << '[' << inf.name() << ", flags: 0x" << std::hex << inf.flags().value() << std::dec << ", MTS: " << inf.max_texture_size() << ']';
}