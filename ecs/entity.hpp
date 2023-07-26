#pragma once

#include <iostream>

#include "component.hpp"

namespace ecs
{
    class scene;

    class entity
    {
        friend class scene;  // Manages assigned components.

      public:

        using index   = lyo::u16;  // Max ents >= 65535.
        using version = lyo::u16;

        struct ID
        {
            constexpr ID(index idnum, version vernum) noexcept :
                idx { idnum },
                ver { vernum }
            {
            }

            // This'll probably act differently when fast types are enabled...
            constexpr bool valid() const noexcept
            {
                return idx != static_cast<index>(-1);
            }

            constexpr bool operator==(const ID other) const noexcept
            {
                return idx == other.idx && ver == other.ver;
            }

            constexpr bool operator!=(const ID other) const noexcept
            {
                return idx != other.idx || ver != other.ver;
            }

            index   idx;
            version ver;
        };

        constexpr entity(index idnum, version vernum) noexcept :
            id { idnum, vernum }
        {
        }

        constexpr ID get_ID() const noexcept
        {
            return id;
        }

        constexpr component::mask get_mask() const noexcept
        {
            return mask;
        }

        void print_comps() const noexcept
        {
            std::cout << mask << '\n';
        }

        constexpr static index invalid_index() noexcept
        {
            return static_cast<index>(-1);
        }

      private:

        component::mask mask;

        ID id;
    };
}  // namespace ecs