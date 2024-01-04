#pragma once

#include <HalQ/app.hpp>
#include <HalQ/manager.hpp>

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

        manager<128> m_entityMgr;
    };
}