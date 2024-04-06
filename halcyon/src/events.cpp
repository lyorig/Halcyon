#include <halcyon/debug.hpp>
#include <halcyon/events.hpp>

using namespace hal;

event::event()
{
    // Disable unused events.
    for (SDL_EventType type : {
             SDL_LOCALECHANGED,
             SDL_SYSWMEVENT,
             SDL_TEXTEDITING,
             SDL_TEXTINPUT,
             SDL_KEYMAPCHANGED,
             SDL_TEXTEDITING_EXT,
             SDL_JOYAXISMOTION,
             SDL_JOYBALLMOTION,
             SDL_JOYHATMOTION,
             SDL_JOYBUTTONDOWN,
             SDL_JOYBUTTONUP,
             SDL_JOYDEVICEADDED,
             SDL_JOYDEVICEREMOVED,
             SDL_JOYBATTERYUPDATED,
             SDL_CONTROLLERAXISMOTION,
             SDL_CONTROLLERBUTTONDOWN,
             SDL_CONTROLLERBUTTONUP,
             SDL_CONTROLLERDEVICEADDED,
             SDL_CONTROLLERDEVICEREMOVED,
             SDL_CONTROLLERDEVICEREMAPPED,
             SDL_CONTROLLERTOUCHPADDOWN,
             SDL_CONTROLLERTOUCHPADMOTION,
             SDL_CONTROLLERTOUCHPADUP,
             SDL_CONTROLLERSENSORUPDATE,
             SDL_CONTROLLERSTEAMHANDLEUPDATED,
             SDL_FINGERDOWN,
             SDL_FINGERUP,
             SDL_FINGERMOTION,
             SDL_DOLLARGESTURE,
             SDL_DOLLARRECORD,
             SDL_MULTIGESTURE,
             SDL_DROPFILE,
             SDL_DROPTEXT,
             SDL_DROPBEGIN,
             SDL_DROPCOMPLETE,
             SDL_AUDIODEVICEADDED,
             SDL_AUDIODEVICEREMOVED,
             SDL_SENSORUPDATE,
             SDL_RENDER_TARGETS_RESET,
             SDL_RENDER_DEVICE_RESET })
        ::SDL_EventState(type, SDL_IGNORE);
}

bool event::poll()
{
    return ::SDL_PollEvent(reinterpret_cast<SDL_Event*>(&m_event.data)) == 1;
}

event::event_type event::type() const
{
    return static_cast<event_type>(m_event.data.type);
}

bool event::pending()
{
    return ::SDL_PollEvent(nullptr) == 1;
}

const events::display& event::display() const
{
    HAL_ASSERT(type() == display_event, "Invalid type");
    return m_event.data.display;
}

const events::window& event::window() const
{
    HAL_ASSERT(type() == window_event, "Invalid type");
    return m_event.data.window;
}

const events::keyboard& event::keyboard() const
{
    HAL_ASSERT(type() == key_pressed || type() == key_released, "Invalid type");
    return m_event.data.key;
}

const events::mouse_motion& event::motion() const
{
    HAL_ASSERT(type() == mouse_motion, "Invalid type");
    return m_event.data.motion;
}

const events::mouse_button& event::button() const
{
    HAL_ASSERT(type() == mouse_pressed || type() == mouse_released, "Invalid type");
    return m_event.data.button;
}

const events::mouse_wheel& event::wheel() const
{
    HAL_ASSERT(type() == mouse_wheel, "Invalid type");
    return m_event.data.wheel;
}