#pragma once

#include <halcyon/halcyon.hpp>
#include <lyoECS/entity.hpp>
#include <lyoECS/scene.hpp>
#include <lyoSTL/argparse.hpp>
#include <quest/types.hpp>

namespace quest
{
    namespace constants
    {
        constexpr hal::pixel_t logical_width { 1920 };
    }

    class game
    {
    public:
        using scene = lyo::ECS::static_scene<holder, lyo::ECS::static_entity, 128>;

        game(lyo::parser&& args);

        void intro();

        void start();

    private:
        LYOSTL_NOSIZE hal::engine m_eng;
        LYOSTL_NOSIZE hal::video m_video;
        LYOSTL_NOSIZE hal::audio m_audio;
        LYOSTL_NOSIZE hal::image_loader m_image;
        LYOSTL_NOSIZE hal::ttf_engine m_ttf;

        scene m_entityMgr;

        hal::queued_input_handler m_input;

        const lyo::parser m_args;
        const hal::font   m_font;

        hal::mixer    m_mixer;
        hal::window   m_window;
        hal::renderer m_renderer;

        lyo::precise_timer m_delta;

        bool update();

        lyo::f64 delta() const;
    };
}