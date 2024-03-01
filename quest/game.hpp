#pragma once

#include <lyo/argparse.hpp>
#include <quest/entity_manager.hpp>

// game.hpp:
// The HalQuest game class.

namespace quest
{
    class game
    {
    public:
        game(lyo::parser p);

        void main_loop();

    private:
        // Players are a singleton object that is implicitly a part
        // of the entity manager.
        using emgr = entity_manager<light, bullet_shell>;

        LYO_NOSIZE hal::cleanup m_cleanup;
        LYO_NOSIZE hal::image_loader m_image;

        hal::window   m_window;
        hal::renderer m_renderer;

        emgr m_ents;

        void process_press(hal::button b);
        void process_release(hal::button b);
    };
};