#pragma once

#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>

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

        lyo::f64 delta() const;

        hal::texture load_pixel_art(const hal::surface& surf);

        // The height percentage should be passed in percentage form, e.g. 12.5 instead of 0.125.
        hal::font load_appx_font(const char* path, lyo::f64 screen_height_percentage);

        // The width percentage should be passed in percentage form, e.g. 50.0 instead of 0.5.
        hal::font load_appx_font(const char* path, const char* example_text, lyo::f64 screen_width_percentage);

        hal::pixel_point apx_size();

        LYO_NOSIZE hal::engine m_eng;
        LYO_NOSIZE hal::video m_video;
        LYO_NOSIZE hal::audio m_audio;
        LYO_NOSIZE hal::image_loader m_image;
        LYO_NOSIZE hal::ttf_engine m_ttf;

        hal::queued_input_handler m_input;

        const lyo::parser m_args;
        const hal::font   m_font;

        hal::mixer    m_mixer;
        hal::window   m_window;
        hal::renderer m_renderer;

        lyo::precise_timer m_delta;

        hal::target_texture m_canvas;
    };
}