#pragma once

#include <SDL2/SDL.h>

#include "components/display_info.hpp"

/* core.hpp:
   The core of Halcyon, without which nothing can be initialized. */

namespace hal
{
    class engine
    {
      public:

        engine() noexcept;
        ~engine();

        display_info monitor_info(lyo::u8 index) const noexcept;

        void exit() noexcept;

      private:

        void deinitialize() noexcept;
    };
}  // namespace halcyon