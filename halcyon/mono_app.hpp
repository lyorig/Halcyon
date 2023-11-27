#pragma once

#include "engine.hpp"
#include "image_loader.hpp"
#include "input_handler.hpp"
#include "lyo/types.hpp"
#include "mixer.hpp"
#include "ttf_engine.hpp"
#include "window.hpp"

namespace hal
{

    // An example template for a single-window application.
    class mono_app
    {
        MAYBE_EMPTY engine m_eng;

        input_handler m_input;

      public:

        class mixer mixer;

        class window window;

        MAYBE_EMPTY image_loader image;
        MAYBE_EMPTY ttf_engine   ttf;

        // TODO: Add more constructor options in a... humane way.
        mono_app(const char* window_name) noexcept;

        bool update() noexcept;

        const input_handler& input() const noexcept;
    };
}  // namespace hal