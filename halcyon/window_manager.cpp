#include "window_manager.hpp"

using namespace hal;

window &window_manager::operator[](window::id_type id) noexcept {
    auto iter = this->find_by_id(id);

    HAL_DEBUG_CHECK(iter != m_windows.end(), "Window ID doesn't exist");

    return *iter;
}

bool window_manager::update() noexcept
{
    for (window& wnd : m_windows)
        wnd.present();

    return input_handler::update();
}

std::vector<window>::iterator window_manager::find_by_id(window::id_type id) noexcept
{
    return std::find_if(m_windows.begin(), m_windows.end(), [&](const window& wnd){return wnd.id() == id;});
}

bool window_manager::process(const SDL_Event& event) noexcept
{
    switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.repeat == 0)
                m_pressed.set(event.key.keysym.scancode);

            break;

        case SDL_KEYUP:
            if (event.key.repeat == 0)
                m_released.set(event.key.keysym.scancode);

            break;

        case SDL_MOUSEBUTTONDOWN:
            m_pressed.set(SDL_NUM_SCANCODES - 1 + event.button.button);
            break;

        case SDL_MOUSEBUTTONUP:
            m_released.set(SDL_NUM_SCANCODES - 1 + event.button.button);
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.type)
            {
                case SDL_WINDOWEVENT_CLOSE:
                    m_windows.erase(this->find_by_id(event.window.windowID));
                    break;

                default:
                    break;
            }
            break;

        case SDL_QUIT:
            return false;

        default:
            break;
    }

    return true;
}
