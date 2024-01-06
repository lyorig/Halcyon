#pragma once

#include <ecs/entity.hpp>
#include <ecs/scene.hpp>
#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>
#include <types.hpp>

namespace hq
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

        using scene = ECS::static_scene<holder, ECS::static_entity, 128>;

        LYO_MAYBE_EMPTY hal::engine m_eng;
        LYO_MAYBE_EMPTY hal::video m_video;
        LYO_MAYBE_EMPTY hal::audio m_audio;
        LYO_MAYBE_EMPTY hal::image_loader m_image;
        LYO_MAYBE_EMPTY hal::ttf_engine m_ttf;

        scene m_entityMgr;

        hal::queued_input_handler m_input;

        const lyo::parser m_args;

        class hal::mixer m_mixer;

        class hal::window   m_window;
        class hal::renderer m_renderer;

        lyo::precise_timer m_delta;
    };
}