#include <halcyon/other/printing.hpp>
#include <halcyon/other/subsystem.hpp>

using namespace hal;

// The linker takes issue with this not being explicitly namespaced.
std::ostream& hal::operator<<(std::ostream& str, const display& disp)
{
    return str << "[Display " << to_printable_int(disp.idx()) << ", " << disp.size() << ", " << to_printable_int(disp.hz()) << " Hz]";
}