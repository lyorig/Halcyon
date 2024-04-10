#pragma once

#include <lyo/argparse.hpp>

#include <halcyon/halcyon.hpp>

#include <quest/camera.hpp>
#include <quest/entity_manager.hpp>

// game.hpp:
// The HalQuest game class.

namespace quest
{
    class game
    {
    public:
        game();

        void main_loop();

    private:
        entity_manager m_ents;

        LYO_NOSIZE hal::cleanup m_cleanup { hal::system::video };
        LYO_NOSIZE hal::image::cleanup m_image { hal::image::format::png };
        LYO_NOSIZE hal::ttf::cleanup m_ttf;

        hal::window   m_window { "HalodaQuest", hal::fullscreen };
        hal::renderer m_renderer { m_window, { hal::renderer::flags::accelerated, hal::renderer::flags::vsync } };

        camera m_cam;

        animatable<lyo::f64, easing::in_out::parametric> m_timescale { 1.0 };

        // Returns whether to continue running.
        bool process_events(hal::event_handler& event);

        void process_press(hal::keyboard::button b);
        void process_release(hal::keyboard::button b);
    };
};
