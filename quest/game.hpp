#pragma once

#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>
#include <lyo/stack_polymorphism.hpp>
#include <quest/states/intro.hpp>
#include <quest/states/menu.hpp>
#include <quest/states/paused.hpp>
#include <quest/states/playing.hpp>

namespace quest
{
    class game
    {
    public:
        game(lyo::parser&& args);

        void intro();
        void start();

    private:
        bool update();
        void dispatch_event(state::type nt);

        lyo::f64 delta() const;

        hal::texture load_pixel_art(const hal::surface& surf);

        // The height percentage should be passed in percentage form, e.g. 12.5 instead of 0.125.
        hal::font load_appx_font(const char* path, lyo::f64 screen_height_percentage);

        // The width percentage should be passed in percentage form, e.g. 50.0 instead of 0.5.
        hal::font load_appx_font(const char* path, const char* example_text, lyo::f64 screen_width_percentage);

        hal::pixel_point apx_size();

        // Systems.
        LYO_NOSIZE hal::engine m_eng;
        LYO_NOSIZE hal::video m_video;
        LYO_NOSIZE hal::image_loader m_image;
        LYO_NOSIZE hal::ttf_engine m_ttf;

        hal::queued_input_handler m_input;

        // Constants.
        const lyo::parser m_args;
        const hal::font   m_font;

        // Audio.
        hal::audio::listener m_list;

        // Rendering. [audio is currently WIP]
        hal::window   m_window;
        hal::renderer m_renderer;

        // Game states.
        state::base* m_state;

        state::menu    m_menuState;
        state::paused  m_pausedState;
        state::playing m_playingState;

        lyo::precise_timer m_delta;
    };
}