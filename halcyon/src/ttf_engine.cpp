#include <halcyon/ttf_engine.hpp>

using namespace hal;

ttf_engine::ttf_engine()
{
    HAL_ASSERT_VITAL(::TTF_Init() == 0, ::TTF_GetError());

    HAL_PRINT(debug::init, "Initialized TTF engine");
}

ttf_engine::~ttf_engine() { ::TTF_Quit(); }

bool ttf_engine::initialized()
{
    return ::TTF_WasInit() > 0;
}