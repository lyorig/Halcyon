#pragma once

#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>

namespace hq
{
    class app
    {
        LYO_MAYBE_EMPTY hal::engine m_eng;

        LYO_MAYBE_EMPTY hal::video m_video;
        LYO_MAYBE_EMPTY hal::audio m_audio;

    public:
        hal::input_handler input;

        class hal::mixer mixer;

        class hal::window window;

        const lyo::parser args;

        LYO_MAYBE_EMPTY hal::image_loader image;
        LYO_MAYBE_EMPTY hal::ttf_engine ttf;

        app(lyo::parser&& args, const char* window_name);

        bool update();

        lyo::f64 delta() const;

    private:
        lyo::precise_timer m_delta;
    };
} // namespace hal