#pragma once

#include <SDL_rwops.h>

// internal/rwops.hpp:
// Common base class for SDL_RWops functionality.

namespace hal::sdl
{
    class rwops
    {
    public:
        rwops(const rwops&) = delete;
        rwops(rwops&&)      = default;

        rwops(SDL_RWops* ptr);

    protected:
        SDL_RWops* m_ptr;
    };
}