#pragma once

#include <string_view>

#include <SDL_ttf.h>

#include <halcyon/internal/rwops.hpp>
#include <halcyon/surface.hpp>

// ttf.hpp:
// SDL_ttf wrappers for font loading and text rendering.

namespace hal
{
    namespace builder
    {
        class font_text;
        class font_glyph;
    }

    class font : public detail::raii_object<TTF_Font, &::TTF_CloseFont>
    {
    public:
        using pt_t = u8;

        enum class render_type : u8
        {
            solid,
            shaded,
            blended,
            lcd
        };

        // [private] Fonts are loaded with ttf::context::load().
        font(TTF_Font* ptr, pass_key<ttf::context>);

        // Render text to a surface.
        [[nodiscard]] builder::font_text render(std::string_view text) const;

        // Render a single glyph to a surface.
        [[nodiscard]] builder::font_glyph render(std::uint32_t glyph) const;

        // When sizing text, it's important to know that the vertical size
        // doesn't necessarily have to match that of the rendered surface.
        pixel_point size_text(const std::string_view& text) const;

        pixel_t height() const;
        pixel_t skip() const;

        std::string_view family() const;
        std::string_view style() const;
    };

    constexpr std::string_view to_string(font::render_type rt)
    {
        switch (rt)
        {
            using enum font::render_type;

        case solid:
            return "Solid";

        case shaded:
            return "Shaded";

        case blended:
            return "Blended";

        case lcd:
            return "LCD";
        }

        std::unreachable();
    }

    namespace ttf
    {
        // A class that makes sure everything TTF-related is loaded and
        // ready to use. This includes not only loading fonts, but also
        // their features - for example, font::render() will fail if a
        // TTF engine doesn't exist. TL;DR: Ensure that this object outlives
        // all fonts. This includes destructors!
        class context
        {
        public:
            // Initialize the TTF context.
            context();

            context(const context&) = delete;
            context(context&&)      = delete;

            ~context();

            // Font loading function.
            [[nodiscard]] font load(accessor data, font::pt_t pt) &;

            static bool initialized();
        };

        static_assert(std::is_empty_v<context>);
    }

    namespace detail
    {
        template <typename Derived>
        class font_builder_base
        {
        public:
            [[nodiscard]] font_builder_base(const hal::font& fnt, pass_key<font>)
                : m_font { fnt }
                , m_fg { hal::palette::white }
                , m_bg { hal::palette::transparent }
            {
            }

            // Set the foreground (text) color.
            [[nodiscard]] Derived& fg(color c)
            {
                m_fg = c;

                return get_this();
            }

            // Set the background color.
            // Does not have an effect on all render types.
            [[nodiscard]] Derived& bg(color c)
            {
                m_bg = c;

                return get_this();
            }

        protected:
            Derived& get_this()
            {
                return static_cast<Derived&>(*this);
            }

            const hal::font& m_font;
            color            m_fg, m_bg;
        };
    };

    namespace builder
    {
        class font_text : public detail::font_builder_base<font_text>
        {
        public:
            using wrap_length_t = u16;

            [[nodiscard]] font_text(const font& fnt, std::string_view text, pass_key<font>);

            // How many characters to wrap this text at.
            // Zero means only wrap on newlines.
            [[nodiscard]] font_text& wrap(wrap_length_t wl);

            [[nodiscard]] surface operator()(font::render_type rt = font::render_type::solid);

        private:
            consteval static wrap_length_t invalid()
            {
                return std::numeric_limits<wrap_length_t>::max();
            }

            const char*   m_text;
            wrap_length_t m_wrapLength;
        };

        class font_glyph : public detail::font_builder_base<font_glyph>
        {
        public:
            [[nodiscard]] font_glyph(const font& fnt, std::uint32_t glyph, pass_key<font>);

            [[nodiscard]] surface operator()(font::render_type rt = font::render_type::solid);

        private:
            std::uint32_t m_glyph;
        };
    }

    // Ensure calling debug::last_error() gives accurate information.
    static_assert(::TTF_GetError == ::SDL_GetError);
}