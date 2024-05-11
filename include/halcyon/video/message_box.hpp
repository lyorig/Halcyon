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
    }

    namespace detail
    {
        class message_box_builder
        {
            using this_ref = message_box_builder&;

        public:
            message_box_builder();

            [[nodiscard]] this_ref title(std::string_view text);
            [[nodiscard]] this_ref message(std::string_view text);

            [[nodiscard]] this_ref type(message_box::type tp);
            [[nodiscard]] this_ref buttons(std::initializer_list<std::string_view> names);

            [[nodiscard]] this_ref enter(message_box::button_t id);
            [[nodiscard]] this_ref escape(message_box::button_t id);

            [[nodiscard]] this_ref parent(window& wnd);

            // Create the message box.
            // Returns the ID of the button that was pressed.
            message_box::button_t operator()();

        private:
            std::array<SDL_MessageBoxButtonData, 3> m_btn;

            SDL_MessageBoxData m_data;
        };
    }

    namespace message_box
    {
        [[nodiscard]] detail::message_box_builder build();
    };
}