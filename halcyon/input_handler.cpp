#include "input_handler.hpp"

#include <SDL2/SDL_events.h>

#include "debug.hpp"
#include "engine.hpp"

using namespace hal;

input_handler::input_handler(engine& engine) noexcept :
    m_shouldQuit { false }
{
    HAL_DEBUG_PRINT(severity::init, "Initialized input handler");
}

void input_handler::update() noexcept
{
    m_pressed.clear();
    m_released.clear();

    SDL_Event event;

    while (::SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
            {
                const SDL_Scancode key { event.key.keysym.scancode };

                if (event.key.repeat == 0)
                {
                    m_pressed.set(key);
                    m_held.set(key);
                }

                break;
            }

            case SDL_KEYUP:
            {
                const SDL_Scancode key { event.key.keysym.scancode };

                if (event.key.repeat == 0)
                {
                    m_held.reset(key);
                    m_released.set(key);
                }

                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                const auto btn { event.button.button };

                m_pressed.set(SDL_NUM_SCANCODES - 1 + btn);
                m_held.set(SDL_NUM_SCANCODES - 1 + btn);

                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                const auto btn { event.button.button };

                m_held.reset(SDL_NUM_SCANCODES - 1 + btn);
                m_released.set(SDL_NUM_SCANCODES - 1 + btn);

                break;
            }

            case SDL_QUIT:
                m_shouldQuit = true;
                break;

            default:

                break;
        }
    }

    // Lastly, get the mouse's position.
    int x, y;
    ::SDL_GetMouseState(&x, &y);
    mouse.x = x, mouse.y = y;
}

bool input_handler::pressed(button btn) const noexcept
{
    return m_pressed[static_cast<lyo::usize>(btn)];
}

bool input_handler::held(button btn) const noexcept
{
    return m_held[static_cast<lyo::usize>(btn)];
}

bool input_handler::released(button btn) const noexcept
{
    return m_released[static_cast<lyo::usize>(btn)];
}

bool input_handler::should_quit() const noexcept
{
    return m_shouldQuit;
}
