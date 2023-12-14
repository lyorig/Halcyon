#pragma once

#include <halcyon/engine.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>
#include <lyo/argparse.hpp>

namespace hq
{
    class app
    {
        MAYBE_EMPTY hal::engine m_eng;

        hal::input_handler m_input;

    public:
        class hal::mixer mixer;

        class hal::window window;

        const lyo::parser args;

        MAYBE_EMPTY hal::image_loader image;
        MAYBE_EMPTY hal::ttf_engine ttf;

        app(const lyo::parser& args, const char* window_name);

        bool update();

        const hal::input_handler& input() const;

        lyo::f64 delta() const;

    private:
        lyo::precise_timer m_delta;
    };
} // namespace hal