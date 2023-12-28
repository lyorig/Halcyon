#include <halcyon/internal/printing.hpp>
#include <halcyon/internal/subsystem.hpp>

using namespace hal;

std::ostream& operator<<(std::ostream& str, const display& disp)
{
    return str << "[Display " << disp.index << ", " << disp.size() << ", " << disp.hz() << " Hz]";
}