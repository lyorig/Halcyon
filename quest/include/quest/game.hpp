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
        LYO_NOSIZE hal::image_loader m_image { hal::image_loader::png };

        hal::window   m_window { "HalodaQuest", hal::fullscreen };
        hal::renderer m_renderer { m_window, { hal::renderer::accelerated, hal::renderer::vsync } };

        camera m_cam;

        animatable<lyo::f64, easing::in_out::parametric> m_timescale { 1.0 };

        bool process_events(hal::event& event);

        bool process_press(hal::button b);
        void process_release(hal::button b);
    };
};
