#pragma once

#include <SDL2/SDL.h>

/* core.hpp:
   The core of Halcyon, without which nothing can be initialized. */

namespace hal {
class engine {
public:
    engine();
    ~engine();

    void exit() &;

private:
    void deinitialize() const;
};
} // namespace halcyon