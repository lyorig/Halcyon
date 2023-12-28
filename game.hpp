#pragma once

#include <app.hpp>
#include <ents/manager.hpp>

namespace hq
{
    class game
    {
    public:
        game(lyo::parser&& args);

        void intro();
        void start();

    private:
        class app app;

        entity_manager m_emgr;
    };
}