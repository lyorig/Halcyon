#include <halcyon/internal/printing.hpp>
#include <halcyon/internal/subsystem.hpp>

using namespace hal;

// The linker takes issue with this not being explicitly namespaced.
std::ostream& hal::operator<<(std::ostream& str, const display& disp)
{
    return str << "[Display " << make_printable_int(disp.idx()) << ", " << disp.size() << ", " << make_printable_int(disp.hz()) << " Hz]";
}