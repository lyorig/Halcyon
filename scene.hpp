#pragma once

#include <halcyon/font_loader.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/window.hpp>

#include "level.hpp"

/* scene.hpp:
   An all-in-one package with everything needed for the game. */

namespace hal
{
    class engine;
}

namespace game
{
    class entity;

    class scene
    {
      public:

        scene(hal::engine& eng, const char* game_title);

        // @brief Update the scene and all its components.
        // @returns Whether the game should quit. This way, you can
        // wrap this function in a while (scene.update()) block.
        bool update() noexcept;

        void load_level(const char* path) noexcept;

      private:

        hal::input_handler m_input;

        hal::window m_window;

        level m_level;

        lyo::precise_timer m_delta;

        MAYBE_EMPTY hal::image_loader m_iLoader;
        MAYBE_EMPTY hal::font_loader m_fLoader;
    };
}  // namespace game