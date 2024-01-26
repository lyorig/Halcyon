#include <halcyon/debug.hpp>

#ifdef HAL_DEBUG_ENABLED
    #include <SDL2/SDL_messagebox.h>

    #include <halcyon/components/font.hpp>
    #include <halcyon/internal/printing.hpp>
    #include <halcyon/renderer.hpp>
    #include <halcyon/texture.hpp>
    #include <lyo/utility.hpp>

using namespace hal;

// Static private variables.
std::ofstream            debug::m_output { "Halcyon debug output.txt" };
const lyo::precise_timer debug::m_timer {};

debug::queue_type debug::m_queue {};
debug::count_type debug::m_entries { 0 };

bool debug::m_repaint { false };

void debug::draw(renderer& rnd, const font& fnt)
{
    using namespace literals;

    // Render settings.
    constexpr coord_point offset { 20.0_crd, 20.0_crd };

    // I am not 100% sure whether it's safe to create a static
    // SDL object, as the destructor will run after de-initialization.
    // However, it doesn't crash, it's fast, and it's debug, so who cares.
    static target_texture tx;

    if (m_repaint)
    {
        static pixel_t y_size { fnt.render(".").size().y };

        pixel_point csz { .y = pixel_t(y_size * m_entries) }; // Canvas size.

        // "Calculate" canvas size.
        for (count_type i { 0 }; i < m_entries; ++i)
        {
            const pixel_t x_size { fnt.size_text(m_queue[i].first).x };
            if (x_size > csz.x)
                csz.x = x_size;
        }

        if (csz.x != 0) [[likely]]
        {
            tx.resize(rnd, csz);

            hal::target_lock lock { rnd, tx };

            // Compose the texture.
            for (count_type i { 0 }; i < m_entries; ++i)
            {
                const value_pair& entry { m_queue[i] };

                if (!entry.first.empty()) [[likely]]
                {
                    const hal::texture text { rnd, fnt.render(entry.first, entry.second) };
                    const pixel_point  pos { 0, pixel_t(i * y_size) };

                    rnd.draw(text).to(pos)();
                }
            }
        }

        m_repaint = false;
    }

    rnd.draw(tx).to(offset)();
}

void debug::panic(const char* why, const char* where,
    const char* message)
{
    const char* sanitized_msg { message && !lyo::is_c_string_empty(message) ? message : "None provided." };

    debug::print_severity(error, __func__, ": ", why, " in ", where, ": ",
        sanitized_msg);

    constexpr SDL_MessageBoxButtonData buttons[] {
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Exit" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Run anyway" }
    };

    const std::string msgbox_info {
        lyo::string_from_pack("Function: ", where, "\nInfo: ", sanitized_msg)
    };

    const SDL_MessageBoxData msgbox {
        SDL_MESSAGEBOX_ERROR, nullptr, why, msgbox_info.c_str(),
        static_cast<int>(std::size(buttons)), buttons, nullptr
    };

    int response { 0 };

    if (::SDL_ShowMessageBox(&msgbox, &response) < 0) [[unlikely]]
    {
        debug::print_severity(error, __func__,
            ": Message box creation failed, exiting");
        goto Exit;
    }

    else
        debug::print_severity(info, __func__, ": User chose to ",
            response == 0 ? "exit" : "continue execution");

    if (response == 0) [[likely]]
    {
    Exit:
        std::exit(EXIT_FAILURE);
    }
}

void debug::verify(bool condition, const char* cond_string, const char* func,
    const char* extra_info)
{
    if (!condition) [[unlikely]]
        debug::panic(cond_string, func, extra_info);
}

void debug::log(severity type, const std::string& msg)
{
    if (m_entries == m_queue.size()) [[likely]]
    {
        std::rotate(m_queue.begin(), m_queue.begin() + 1, m_queue.end());
        m_queue.back() = { msg, type };
    }

    else
        m_queue[m_entries++] = { msg, type };

    m_repaint = true;
}
#endif