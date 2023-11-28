#pragma once

#include <SDL2/SDL_events.h>

#include <halcyon/enums/buttons.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/bitset.hpp>

namespace hal {
class engine;

class input_base {
public:
    using key_storage = lyo::bitset<SDL_NUM_SCANCODES + 5, lyo::u64>;

    explicit input_base(engine& eng) noexcept;

    // Automatic polling. If you want finer control (or use
    // functions not yet implemented), use the poll() function.
    virtual bool update() noexcept;

    // This function doesn't require any storage, so it's
    // included as part of the base.
    bool held(hal::button btn) const noexcept;

    pixel_pos mouse() const noexcept;

protected:
    bool poll(SDL_Event& event) const noexcept;
    virtual bool process(const SDL_Event& event) noexcept = 0;
};

// A basic enough input handler. Suitable for a single-window
// application. Extend if necessary.
class input_handler : public input_base {
public:
    using input_base::input_base;

    bool update() noexcept;

    bool pressed(hal::button btn) const noexcept;

    bool released(hal::button btn) const noexcept;

protected:
    key_storage m_pressed, m_released;

    virtual bool process(const SDL_Event& event) noexcept;
};
} // namespace hal