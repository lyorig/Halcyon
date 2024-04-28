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

        void free() const;

    protected:
        SDL_RWops* m_ptr;
    };
}