#include "entity_manager.hpp"
#include <halcyon/halcyon.hpp>

int main(int argc, char* argv[])
{
    (void)argc, (void)argv;

    using namespace quest;
    entity_manager<player, bullet_shell, light> ents;

    hal::cleanup  c;
    hal::window   wnd { "HQuest", hal::fullscreen_mode };
    hal::renderer rnd { wnd, { hal::renderer::accelerated } };

    hal::event e;

    while (true)
    {
        while (e.poll())
        {
            switch (e.data.type)
            {
            case hal::event::quit_requested:
                goto GTFO;
            }
        }

        ents.visit([&]<updateable T>(const T& ent)
            { ent.update(); });
    }

GTFO:

    return EXIT_SUCCESS;
}