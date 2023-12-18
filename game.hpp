#pragma once

#include "app.hpp"

namespace hq
{
    class game
    {
    public:
        game(lyo::parser&& args);

        void intro();

        void start();

        class app app;
    };
}