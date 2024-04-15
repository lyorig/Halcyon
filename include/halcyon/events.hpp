#pragma once

#include <halcyon/event/handler.hpp>

#include <halcyon/internal/subsystem.hpp>

// events.hpp:
// Master include file for Halcyon Events, plus the events subsystem.

namespace hal
{
    class events : public detail::subinit<system::events>
    {
    public:
        using subinit::subinit;
    };

    static_assert(std::is_empty_v<events>);
}