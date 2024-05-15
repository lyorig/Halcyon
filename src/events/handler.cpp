// Due to strcpy. Don't worry, it's used "safely" here.
#define _CRT_SECURE_NO_WARNINGS

#include <halcyon/debug.hpp>
#include <halcyon/events/handler.hpp>

using namespace hal;

// Display event.

enum event::display_event::type event::display_event::event_type() const
{
    return static_cast<type>(event);
}

event::display_event& event::display_event::event_type(type t)
{
    event = std::to_underlying(t);

    return *this;
}

display::id_t event::display_event::display_index() const
{
    return static_cast<display::id_t>(display);
}

event::display_event& event::display_event::display_index(display::id_t disp)
{
    display = disp;

    return *this;
}

// Window event.

enum event::window_event::type event::window_event::event_type() const
{
    return static_cast<type>(event);
}

event::window_event& event::window_event::event_type(type t)
{
    event = std::to_underlying(t);

    return *this;
}

window::id_t event::window_event::window_id() const
{
    return static_cast<window::id_t>(windowID);
}

event::window_event& event::window_event::window_id(window::id_t id)
{
    windowID = id;

    return *this;
}

display::id_t event::window_event::display_index() const
{
    HAL_ASSERT(event_type() == type::display_changed, "Invalid event type");

    return static_cast<display::id_t>(data1);
}

event::window_event& event::window_event::display_index(display::id_t id)
{
    event_type(type::display_changed);

    data1 = id;

    return *this;
}

pixel_point event::window_event::point() const
{
    HAL_ASSERT(event_type() == type::resized || event_type() == type::moved, "Invalid event type");

    return { static_cast<pixel_t>(data1), static_cast<pixel_t>(data2) };
}

event::window_event& event::window_event::point(pixel_point pt, type t)
{
    HAL_ASSERT(t == type::resized || t == type::moved, "Invalid event setter");

    event_type(t);

    data1 = pt.x;
    data2 = pt.y;

    return *this;
}

// Keyboard event.

window::id_t event::keyboard_event::window_id() const
{
    return static_cast<window::id_t>(windowID);
}

event::keyboard_event& event::keyboard_event::window_id(window::id_t id)
{
    windowID = id;

    return *this;
}

keyboard::button event::keyboard_event::button() const
{
    return static_cast<keyboard::button>(keysym.scancode);
}

event::keyboard_event& event::keyboard_event::button(keyboard::button btn)
{
    keysym.scancode = static_cast<SDL_Scancode>(btn);

    return *this;
}

keyboard::key event::keyboard_event::key() const
{
    return static_cast<keyboard::key>(keysym.sym);
}

event::keyboard_event& event::keyboard_event::key(keyboard::key k)
{
    keysym.sym = static_cast<SDL_KeyCode>(k);

    return *this;
}

bool event::keyboard_event::repeat() const
{
    return static_cast<bool>(SDL_KeyboardEvent::repeat);
}

event::keyboard_event& event::keyboard_event::repeat(bool r)
{
    SDL_KeyboardEvent::repeat = r;

    return *this;
}

// Mouse motion event.

window::id_t event::mouse_motion_event::window_id() const
{
    return static_cast<window::id_t>(windowID);
}

event::mouse_motion_event& event::mouse_motion_event::window_id(window::id_t id)
{
    windowID = id;

    return *this;
}

mouse::state event::mouse_motion_event::state() const
{
    return { SDL_MouseMotionEvent::state, pass_key<mouse_motion_event> {} };
}

event::mouse_motion_event& event::mouse_motion_event::state(mouse::state s)
{
    SDL_MouseMotionEvent::state = s.mask();

    return *this;
}

pixel_point event::mouse_motion_event::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_motion_event& event::mouse_motion_event::pos(pixel_point p)
{
    x = p.x;
    y = p.y;

    return *this;
}

pixel_point event::mouse_motion_event::rel() const
{
    return { static_cast<pixel_t>(xrel), static_cast<pixel_t>(yrel) };
}

event::mouse_motion_event& event::mouse_motion_event::rel(pixel_point p)
{
    xrel = p.x;
    yrel = p.y;

    return *this;
}

// Mouse button event.

window::id_t event::mouse_button_event::window_id() const
{
    return static_cast<window::id_t>(windowID);
}

event::mouse_button_event& event::mouse_button_event::window_id(window::id_t id)
{
    windowID = id;

    return *this;
}

mouse::button event::mouse_button_event::button() const
{
    return static_cast<mouse::button>(SDL_MouseButtonEvent::button);
}

event::mouse_button_event& event::mouse_button_event::button(mouse::button btn)
{
    SDL_MouseButtonEvent::button = std::to_underlying(btn);

    return *this;
}

u8 event::mouse_button_event::click_amount() const
{
    return static_cast<u8>(clicks);
}

event::mouse_button_event& event::mouse_button_event::click_amount(u8 amnt)
{
    clicks = amnt;

    return *this;
}

pixel_point event::mouse_button_event::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_button_event& event::mouse_button_event::pos(pixel_point pt)
{
    x = pt.x;
    y = pt.y;

    return *this;
}

// Mouse wheel event.

window::id_t event::mouse_wheel_event::window_id() const
{
    return static_cast<window::id_t>(windowID);
}

event::mouse_wheel_event& event::mouse_wheel_event::window_id(window::id_t id)
{
    windowID = id;

    return *this;
}

pixel_point event::mouse_wheel_event::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_wheel_event& event::mouse_wheel_event::pos(pixel_point p)
{
    x = p.x;
    y = p.y;

    return *this;
}

point<i16> event::mouse_wheel_event::scroll() const
{
    return { static_cast<i16>(x), static_cast<i16>(y) };
}

event::mouse_wheel_event& event::mouse_wheel_event::scroll(point<i16> s)
{
    x = s.x;
    y = s.y;

    return *this;
}

point<f32> event::mouse_wheel_event::scroll_precise() const
{
    return { static_cast<f32>(preciseX), static_cast<f32>(preciseY) };
}

event::mouse_wheel_event& event::mouse_wheel_event::scroll_precise(point<f32> s)
{
    preciseX = s.x;
    preciseY = s.y;

    return *this;
}

bool event::mouse_wheel_event::scroll_flipped() const
{
    return direction == SDL_MOUSEWHEEL_FLIPPED;
}

event::mouse_wheel_event& event::mouse_wheel_event::scroll_flipped(bool f)
{
    static_assert(SDL_MOUSEWHEEL_NORMAL == false && SDL_MOUSEWHEEL_FLIPPED == true);

    direction = f;

    return *this;
}

// Text editing event.

window::id_t event::text_input_event::window_id() const
{
    return static_cast<window::id_t>(windowID);
}

event::text_input_event& event::text_input_event::window_id(window::id_t id)
{
    windowID = id;

    return *this;
}

std::string_view event::text_input_event::text() const
{
    return SDL_TextInputEvent::text;
}

event::text_input_event& event::text_input_event::text(std::string_view t)
{
    HAL_ASSERT(t.size() <= max_size(), "String too large at ", t.size(), " chars (max: ", max_size(), " chars)");

    // We now know that it's safe to copy this string.
    std::strcpy(SDL_TextInputEvent::text, t.data());

    return *this;
}

// Event handler.

event::handler::handler(authority&)
    : m_event { { std::numeric_limits<std::uint32_t>::max() } } // Start with an invalid event.
{
    // Disable unused events.
    for (SDL_EventType type : {
             SDL_LOCALECHANGED,
             SDL_SYSWMEVENT,
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
             // SDL_CONTROLLERSTEAMHANDLEUPDATED, <- Unsupported on Windows, apparently
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
    {
        ::SDL_EventState(type, SDL_IGNORE);
    }
}

bool event::handler::poll()
{
    return ::SDL_PollEvent(reinterpret_cast<SDL_Event*>(&m_event.m_data)) == 1;
}

void event::handler::push()
{
    HAL_ASSERT_VITAL(::SDL_PushEvent(reinterpret_cast<SDL_Event*>(&m_event.m_data)) >= 0, debug::last_error());
}

event::type event::handler::event_type() const
{
    return static_cast<type>(m_event.m_data.m_type);
}

void event::handler::event_type(event::type t)
{
    m_event.m_data.m_type = std::to_underlying(t);
}

const event::display_event& event::handler::display() const
{
    HAL_ASSERT(event_type() == type::display_event, "Invalid type");

    return m_event.m_data.m_display;
}

event::display_event& event::handler::display()
{
    event_type(type::display_event);

    return m_event.m_data.m_display;
}

const event::window_event& event::handler::window() const
{
    HAL_ASSERT(event_type() == type::window_event, "Invalid type");

    return m_event.m_data.m_window;
}

event::window_event& event::handler::window()
{
    event_type(type::window_event);

    return m_event.m_data.m_window;
}

const event::keyboard_event& event::handler::keyboard() const
{
    HAL_ASSERT(event_type() == type::key_pressed || event_type() == type::key_released, "Invalid type");

    return m_event.m_data.m_key;
}

event::keyboard_event& event::handler::keyboard(type t)
{
    HAL_ASSERT(t == type::key_pressed || t == type::key_released, "Invalid type setter");

    event_type(t);

    return m_event.m_data.m_key;
}

const event::text_input_event& event::handler::text_input() const
{
    HAL_ASSERT(event_type() == type::text_input, "Invalid type");

    return m_event.m_data.m_textInput;
}

event::text_input_event& event::handler::text_input()
{
    event_type(type::text_input);

    return m_event.m_data.m_textInput;
}

const event::mouse_motion_event& event::handler::mouse_motion() const
{
    HAL_ASSERT(event_type() == type::mouse_moved, "Invalid type");

    return m_event.m_data.m_motion;
}

event::mouse_motion_event& event::handler::mouse_motion()
{
    event_type(type::mouse_moved);

    return m_event.m_data.m_motion;
}

const event::mouse_button_event& event::handler::mouse_button() const
{
    HAL_ASSERT(event_type() == type::mouse_pressed || event_type() == type::mouse_released, "Invalid type");

    return m_event.m_data.m_button;
}

event::mouse_button_event& event::handler::mouse_button(type t)
{
    HAL_ASSERT(t == type::mouse_pressed || t == type::mouse_released, "Invalid type setter");

    event_type(t);

    return m_event.m_data.m_button;
}

const event::mouse_wheel_event& event::handler::mouse_wheel() const
{
    HAL_ASSERT(event_type() == type::mouse_wheel_moved, "Invalid type");

    return m_event.m_data.m_wheel;
}

event::mouse_wheel_event& event::handler::mouse_wheel()
{
    event_type(type::mouse_wheel_moved);

    return m_event.m_data.m_wheel;
}

bool event::handler::pending() const
{
    return ::SDL_PollEvent(nullptr) == 1;
}

std::string_view hal::to_string(event::type evt)
{
    using enum event::type;

    switch (evt)
    {
    case quit_requested:
        return "Quit requested";

    case terminated:
        return "Terminated";

    case low_memory:
        return "Low memory";

    case will_enter_background:
        return "Will enter background";

    case entered_background:
        return "Entered background";

    case will_enter_foreground:
        return "Will enter foreground";

    case entered_foreground:
        return "Will enter foreground";

    case display_event:
        return "Display";

    case window_event:
        return "Window";

    case key_pressed:
        return "Key pressed";

    case key_released:
        return "Key released";

    case text_input:
        return "Text input";

    case mouse_moved:
        return "Mouse moved";

    case mouse_pressed:
        return "Mouse pressed";

    case mouse_released:
        return "Mouse released";

    case mouse_wheel_moved:
        return "Mouse wheel moved";

    case clipboard_updated:
        return "Clipboard updated";
    }

    std::unreachable();
}

std::string_view hal::to_string(enum event::display_event::type evt)
{
    using enum event::display_event::type;

    switch (evt)
    {
    case reoriented:
        return "Reoriented";

    case connected:
        return "Connected";

    case disconnected:
        return "Disconnected";

    case moved:
        return "Moved";
    }

    std::unreachable();
}

std::string_view hal::to_string(enum event::window_event::type evt)
{
    using enum event::window_event::type;

    switch (evt)
    {
    case shown:
        return "Shown";

    case hidden:
        return "Hidden";

    case exposed:
        return "Exposed";

    case moved:
        return "Moved";

    case resized:
        return "Resized";

    case minimized:
        return "Minimized";

    case maximized:
        return "Maximized";

    case restored:
        return "Restored";

    case got_mouse_focus:
        return "Got mouse focus";

    case lost_mouse_focus:
        return "Lost mouse focus";

    case got_keyboard_focus:
        return "Got keyboard focus";

    case lost_keyboard_focus:
        return "Lost keyboard focus";

    case closed:
        return "Closed";

    case focus_offered:
        return "Focus offered";

    case hit_test:
        return "Hit test";

    case icc_profile_changed:
        return "ICC profile changed";

    case display_changed:
        return "Display changed";
    }

    std::unreachable();
}