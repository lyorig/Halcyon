// Due to strcpy. Don't worry, it's used "safely" here.
#define _CRT_SECURE_NO_WARNINGS

#include <cstring>

#include <halcyon/debug.hpp>
#include <halcyon/events/holder.hpp>

using namespace hal;

// Display event.

enum event::display::type event::display::kind() const
{
    return static_cast<type>(event);
}

event::display& event::display::kind(type t)
{
    event = std::to_underlying(t);

    return *this;
}

display::id_t event::display::display_index() const
{
    return static_cast<hal::display::id_t>(SDL_DisplayEvent::display);
}

event::display& event::display::display_index(hal::display::id_t disp)
{
    SDL_DisplayEvent::display = disp;

    return *this;
}

// Window event.

enum event::window::type event::window::kind() const
{
    return static_cast<type>(event);
}

event::window& event::window::kind(type t)
{
    event = std::to_underlying(t);

    return *this;
}

window::id_t event::window::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::window& event::window::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

display::id_t event::window::display_index() const
{
    HAL_ASSERT(kind() == type::display_changed, "Invalid event type");

    return static_cast<hal::display::id_t>(data1);
}

event::window& event::window::display_index(hal::display::id_t id)
{
    kind(type::display_changed);

    data1 = id;

    return *this;
}

pixel::point event::window::point() const
{
    HAL_ASSERT(kind() == type::resized || kind() == type::size_changed || kind() == type::moved, "Invalid event type");

    return { static_cast<pixel_t>(data1), static_cast<pixel_t>(data2) };
}

event::window& event::window::point(pixel::point pt, type t)
{
    HAL_ASSERT(t == type::resized || t == type::size_changed || t == type::moved, "Invalid event setter");

    kind(t);

    data1 = pt.x;
    data2 = pt.y;

    return *this;
}

// Keyboard event.

window::id_t event::keyboard::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::keyboard& event::keyboard::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

keyboard::button event::keyboard::button() const
{
    return static_cast<hal::keyboard::button>(keysym.scancode);
}

event::keyboard& event::keyboard::button(hal::keyboard::button btn)
{
    keysym.scancode = static_cast<SDL_Scancode>(btn);

    return *this;
}

keyboard::key event::keyboard::key() const
{
    return static_cast<hal::keyboard::key>(keysym.sym);
}

event::keyboard& event::keyboard::key(hal::keyboard::key k)
{
    keysym.sym = static_cast<SDL_KeyCode>(k);

    return *this;
}

bool event::keyboard::repeat() const
{
    return static_cast<bool>(SDL_KeyboardEvent::repeat);
}

event::keyboard& event::keyboard::repeat(bool r)
{
    SDL_KeyboardEvent::repeat = r;

    return *this;
}

// Mouse motion event.

window::id_t event::mouse_motion::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::mouse_motion& event::mouse_motion::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

mouse::state event::mouse_motion::state() const
{
    return { SDL_MouseMotionEvent::state, pass_key<mouse_motion> {} };
}

event::mouse_motion& event::mouse_motion::state(mouse::state s)
{
    SDL_MouseMotionEvent::state = s.mask();

    return *this;
}

pixel::point event::mouse_motion::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_motion& event::mouse_motion::pos(pixel::point p)
{
    x = p.x;
    y = p.y;

    return *this;
}

pixel::point event::mouse_motion::rel() const
{
    return { static_cast<pixel_t>(xrel), static_cast<pixel_t>(yrel) };
}

event::mouse_motion& event::mouse_motion::rel(pixel::point p)
{
    xrel = p.x;
    yrel = p.y;

    return *this;
}

// Mouse button event.

window::id_t event::mouse_button::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::mouse_button& event::mouse_button::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

mouse::button event::mouse_button::button() const
{
    return static_cast<mouse::button>(SDL_MouseButtonEvent::button);
}

event::mouse_button& event::mouse_button::button(mouse::button btn)
{
    SDL_MouseButtonEvent::button = static_cast<Uint8>(btn);

    return *this;
}

u8 event::mouse_button::click_amount() const
{
    return static_cast<u8>(clicks);
}

event::mouse_button& event::mouse_button::click_amount(u8 amnt)
{
    clicks = amnt;

    return *this;
}

pixel::point event::mouse_button::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_button& event::mouse_button::pos(pixel::point pt)
{
    x = pt.x;
    y = pt.y;

    return *this;
}

// Mouse wheel event.

window::id_t event::mouse_wheel::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::mouse_wheel& event::mouse_wheel::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

pixel::point event::mouse_wheel::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_wheel& event::mouse_wheel::pos(pixel::point p)
{
    x = p.x;
    y = p.y;

    return *this;
}

point<i16> event::mouse_wheel::scroll() const
{
    return { static_cast<i16>(x), static_cast<i16>(y) };
}

event::mouse_wheel& event::mouse_wheel::scroll(point<i16> s)
{
    x = s.x;
    y = s.y;

    return *this;
}

point<f32> event::mouse_wheel::scroll_precise() const
{
    return { static_cast<f32>(preciseX), static_cast<f32>(preciseY) };
}

event::mouse_wheel& event::mouse_wheel::scroll_precise(point<f32> s)
{
    preciseX = s.x;
    preciseY = s.y;

    return *this;
}

bool event::mouse_wheel::scroll_flipped() const
{
    return direction == SDL_MOUSEWHEEL_FLIPPED;
}

event::mouse_wheel& event::mouse_wheel::scroll_flipped(bool f)
{
    static_assert(SDL_MOUSEWHEEL_NORMAL == false && SDL_MOUSEWHEEL_FLIPPED == true);

    direction = f;

    return *this;
}

// Text editing event.

window::id_t event::text_input::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::text_input& event::text_input::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

std::string_view event::text_input::text() const
{
    return SDL_TextInputEvent::text;
}

event::text_input& event::text_input::text(std::string_view t)
{
    HAL_ASSERT(t.size() <= max_size(), "String too large at ", t.size(), " chars (max: ", max_size(), " chars)");

    // We now know that it's safe to copy this string.
    std::strcpy(SDL_TextInputEvent::text, t.data());

    return *this;
}

// Event handler.

event::holder::holder()
    : m_event { { std::numeric_limits<std::uint32_t>::max() } } // Start with an invalid event.
{
}

event::type event::holder::kind() const
{
    return static_cast<type>(m_event.data.type);
}

void event::holder::kind(event::type t)
{
    m_event.data.type = std::to_underlying(t);
}

const event::display& event::holder::display() const
{
    HAL_ASSERT(kind() == type::display_event, "Invalid type");

    return m_event.data.display;
}

event::display& event::holder::display()
{
    HAL_ASSERT(kind() == type::display_event, "Invalid type");

    return m_event.data.display;
}

const event::window& event::holder::window() const
{
    HAL_ASSERT(kind() == type::window_event, "Invalid type");

    return m_event.data.window;
}

event::window& event::holder::window()
{
    HAL_ASSERT(kind() == type::window_event, "Invalid type");

    return m_event.data.window;
}

const event::keyboard& event::holder::keyboard() const
{
    HAL_ASSERT(kind() == type::key_pressed || kind() == type::key_released, "Invalid type");

    return m_event.data.key;
}

event::keyboard& event::holder::keyboard()
{
    HAL_ASSERT(kind() == type::key_pressed || kind() == type::key_released, "Invalid type");

    return m_event.data.key;
}

const event::text_input& event::holder::text_input() const
{
    HAL_ASSERT(kind() == type::text_input, "Invalid type");

    return m_event.data.text_input;
}

event::text_input& event::holder::text_input()
{
    HAL_ASSERT(kind() == type::text_input, "Invalid type");

    return m_event.data.text_input;
}

const event::mouse_motion& event::holder::mouse_motion() const
{
    HAL_ASSERT(kind() == type::mouse_moved, "Invalid type");

    return m_event.data.motion;
}

event::mouse_motion& event::holder::mouse_motion()
{
    HAL_ASSERT(kind() == type::mouse_moved, "Invalid type");

    return m_event.data.motion;
}

const event::mouse_button& event::holder::mouse_button() const
{
    HAL_ASSERT(kind() == type::mouse_pressed || kind() == type::mouse_released, "Invalid type");

    return m_event.data.button;
}

event::mouse_button& event::holder::mouse_button()
{
    HAL_ASSERT(kind() == type::mouse_pressed || kind() == type::mouse_released, "Invalid type");

    return m_event.data.button;
}

const event::mouse_wheel& event::holder::mouse_wheel() const
{
    HAL_ASSERT(kind() == type::mouse_wheel_moved, "Invalid type");

    return m_event.data.wheel;
}

event::mouse_wheel& event::holder::mouse_wheel()
{
    HAL_ASSERT(kind() == type::mouse_wheel_moved, "Invalid type");

    return m_event.data.wheel;
}

bool event::holder::pending() const
{
    return ::SDL_PollEvent(nullptr) == 1;
}

SDL_Event* event::holder::get(pass_key<proxy::events>) const
{
    return reinterpret_cast<SDL_Event*>(const_cast<dummy_event*>(&m_event));
}