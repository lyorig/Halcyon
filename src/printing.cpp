#include <halcyon/printing.hpp>
#include <halcyon/video/display.hpp>

using namespace hal;

// The linker takes issue with this not being explicitly namespaced.
std::ostream& hal::operator<<(std::ostream& str, const display& disp)
{
    return str << '[' << disp.name() << ", index " << hal::to_printable_int(disp.index()) << ", size " << disp.size() << ", " << disp.hz() << " Hz]";
}