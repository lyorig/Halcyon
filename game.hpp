#pragma once

#include <ecs/entity.hpp>
#include <ecs/scene.hpp>
#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>
#include <types.hpp>

namespace hq
{
    namespace constants
    {
        constexpr hal::pixel_size logical_size { 1920, 1080 };
        constexpr lyo::f64        aspect_ratio { logical_size.x / logical_size.y };
    }

    class game
    {
    public:
        using scene = ECS::static_scene<holder, ECS::static_entity, 128>;

        game(lyo::parser&& args);

        void intro();
        void start();

    private:
        LYO_MAYBE_EMPTY hal::engine m_eng;
        LYO_MAYBE_EMPTY hal::video m_video;
        LYO_MAYBE_EMPTY hal::audio m_audio;
        LYO_MAYBE_EMPTY hal::image_loader m_image;
        LYO_MAYBE_EMPTY hal::ttf_engine m_ttf;

        scene m_entityMgr;

        hal::queued_input_handler m_input;

        const lyo::parser m_args;
        const hal::font   m_font;

        hal::mixer      m_mixer;
        hal::window     m_window;
        hal::renderer   m_renderer;

        lyo::precise_timer m_delta;

        bool update();

        lyo::f64 delta() const;
    };
}