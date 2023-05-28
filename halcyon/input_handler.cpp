#include "input_handler.hpp"

#include <SDL2/SDL_events.h>

#include "debug.hpp"
#include "engine.hpp"

using namespace halcyon;

input_handler::input_handler(engine& engine) noexcept :
    m_engine { engine }
{
    HALCYON_PRINT(debug::info, "Initialized input handler.");
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
                m_engine.exit();
                break;

            default:
                // HALCYON_PRINT(debug::info, "Unknown event type received: ", event.type);
                break;
        }
    }
}

bool input_handler::pressed(SDL_Scancode key) const noexcept
{
    return m_pressed[key];
}

bool input_handler::held(SDL_Scancode key) const noexcept
{
    return m_held[key];
}

bool input_handler::released(SDL_Scancode key) const noexcept
{
    return m_released[key];
}