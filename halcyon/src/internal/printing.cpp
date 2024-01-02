#include <halcyon/internal/printing.hpp>
#include <halcyon/internal/subsystem.hpp>

using namespace hal;

// The linker takes issue with this not being explicitly namespaced.
std::ostream& hal::operator<<(std::ostream& str, const display& disp)
{
    return str << "[Display " << to_printable(disp.idx()) << ", " << disp.size() << ", " << to_printable(disp.hz()) << " Hz]";
}