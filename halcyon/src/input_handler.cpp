#include <halcyon/input_handler.hpp>

#include <SDL2/SDL_events.h>

using namespace hal;

bool input_handler::update()
{
    m_pressed.clear();
    m_released.clear();

    return input_base::update();
}

bool input_handler::pressed(button btn) const
{
    return m_pressed[std::size_t(btn)];
}

bool input_handler::released(button btn) const
{
    return m_released[std::size_t(btn)];
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

using qih = queued_input_handler;

bool qih::update()
{
    m_pressed.clear();
    m_released.clear();

    return input_base::update();
}

const qih::holder& qih::pressed() const
{
    return m_pressed;
}

const qih::holder& qih::held() const
{
    return m_held;
}
const qih::holder& qih::released() const
{
    return m_released;
}

bool qih::process(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        if (event.key.repeat == 0)
        {
            const button prs { button(event.key.keysym.scancode) };

            m_pressed.add(prs);
            m_held.add(prs);
        }

        break;

    case SDL_KEYUP:
        if (event.key.repeat == 0)
        {
            const button rel { button(event.key.keysym.scancode) };

            m_held.remove(rel);
            m_released.add(rel);
        }

        break;

    case SDL_MOUSEBUTTONDOWN:
    {
        const button prs { button(SDL_NUM_SCANCODES - 1 + event.button.button) };

        m_pressed.add(prs);
        m_held.add(prs);
    }
    break;

    case SDL_MOUSEBUTTONUP:
    {
        const button rel { button(SDL_NUM_SCANCODES - 1 + event.button.button) };

        m_held.remove(rel);
        m_released.add(rel);
    }

    break;

    case SDL_QUIT:
        return false;
        break;

    default:
        break;
    }

    return true;
}