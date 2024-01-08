#include <halcyon/ttf_engine.hpp>

using namespace hal;

ttf_engine::ttf_engine(video& sys [[maybe_unused]])
{
    HAL_DEBUG_ASSERT_VITAL(::TTF_Init() == 0, ::TTF_GetError());

    HAL_DEBUG_PRINT(debug::init, "Initialized TTF engine");
}

ttf_engine::~ttf_engine() { ::TTF_Quit(); }

font ttf_engine::load(const char* path, lyo::u8 size) const
{
    return { path, size, {} };
}