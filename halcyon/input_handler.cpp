#include "input_handler.hpp"

#include <SDL2/SDL_events.h>

#include <utility>

#include "debug.hpp"

using namespace hal;

input_base::input_base(engine& eng)
{
    HAL_DEBUG_PRINT(severity::init, "Initialized input handler");
}

bool input_base::update()
{
    SDL_Event evt;

    while (this->poll(evt))
        if (!this->process(evt))
            m_ok = false;

    return m_ok;
}

bool input_base::quit() const
{
    return !m_ok;
}

bool input_base::held(button btn) const
{
    if (std::to_underlying(btn) <= SDL_NUM_SCANCODES) // Key.
        return ::SDL_GetKeyboardState(nullptr)[static_cast<lyo::usize>(btn)];

    else // Mouse button.
        return SDL_BUTTON(std::to_underlying(btn) - SDL_NUM_SCANCODES) & ::SDL_GetMouseState(nullptr, nullptr);
}

pixel_pos input_base::mouse() const
{
    point<int> pos;

    ::SDL_GetMouseState(&pos.x, &pos.y);

    return pixel_pos(pos);
}

bool input_base::poll(SDL_Event& event) const
{
    return ::SDL_PollEvent(&event);
}

bool input_handler::update()
{
    m_pressed.clear();
    m_released.clear();

    return input_base::update();
}

bool input_handler::pressed(button btn) const
{
    return m_pressed[static_cast<lyo::usize>(btn)];
}

bool input_handler::released(button btn) const
{
    return m_released[static_cast<lyo::usize>(btn)];
}

bool input_handler::process(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        if (event.key.repeat == 0)
        {
            m_pressed.set(event.key.keysym.scancode);
        }

        break;

    case SDL_KEYUP:
        if (event.key.repeat == 0)
        {
            m_released.set(event.key.keysym.scancode);
        }

        break;

    case SDL_MOUSEBUTTONDOWN:
        m_pressed.set(SDL_NUM_SCANCODES - 1 + event.button.button);
        break;

    case SDL_MOUSEBUTTONUP:
        m_released.set(SDL_NUM_SCANCODES - 1 + event.button.button);
        break;

    case SDL_QUIT:
        return false;
        break;

    default:
        break;
    }

    return true;
}