#pragma once

#include <halcyon/engine.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>
#include <lyo/argparse.hpp>

namespace hq {

// An example template for a single-window application.
class mono_app {
    MAYBE_EMPTY hal::engine m_eng;

    hal::input_handler m_input;

public:
    class hal::mixer mixer;

    class hal::window window;

    MAYBE_EMPTY hal::image_loader image;
    MAYBE_EMPTY hal::ttf_engine ttf;

    mono_app(const lyo::parser& args, const char* window_name);

    bool update();

    const hal::input_handler& input() const;
};
} // namespace hal