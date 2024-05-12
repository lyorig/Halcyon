#pragma once

#include <array>
#include <string_view>

#include <SDL_messagebox.h>

#include <halcyon/video/window.hpp>

namespace hal
{
    namespace message_box
    {
        using button_t = u8;

        constexpr button_t max_buttons() { return 3; }

        enum class type : u8
        {
            info    = SDL_MESSAGEBOX_INFORMATION,
            warning = SDL_MESSAGEBOX_WARNING,
            error   = SDL_MESSAGEBOX_ERROR
        };

        enum class default_key : u8
        {
            none   = 0,
            enter  = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
            escape = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT
        };

        class builder
        {
            using this_ref = builder&;

        public:
            builder();

            // Set the title text of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref title(std::string_view text);

            // Set the body text of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref body(std::string_view text);

            // Set the type of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref type(type tp);

            // Set the amount of buttons along with their contents.
            [[nodiscard]] this_ref buttons(std::initializer_list<std::string_view> names);

            [[nodiscard]] this_ref colors(color bg = hal::palette::black, color text = hal::palette::white, color btn_border = hal::palette::cyan, color btn_bg = hal::palette::black, color btn_select = hal::palette::orange);

            // Set which button is chosen by default upon hitting enter.
            // Call after setting up buttons.
            [[nodiscard]] this_ref enter(button_t idx);

            // Set which button is chosen by default upon hitting escape.
            // Call after setting up buttons.
            [[nodiscard]] this_ref escape(button_t idx);

            // Set this message box's parent window.
            // Can be called at any time.
            [[nodiscard]] this_ref parent(window& wnd);

            // Create the message box.
            // Returns the ID of the button that was pressed.
            button_t operator()();

        private:
            SDL_MessageBoxData        m_data;
            SDL_MessageBoxButtonData  m_btn[max_buttons()];
            SDL_MessageBoxColorScheme m_col;
        };
    };
}