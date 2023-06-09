#pragma once

#include <SDL2/SDL.h>

/* core.hpp:
   The core of Halcyon, without which nothing can be initialized. */

namespace hal
{
    class engine
    {
      public:

        engine() noexcept;
        ~engine();

        void exit() const & noexcept;

      private:

        void deinitialize() const noexcept;
    };
}  // namespace halcyon