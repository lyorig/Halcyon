#include "scene.hpp"

#include <fstream>
#include <halcyon/debug.hpp>

using namespace hq;

scene::scene(const hal::window& wnd, hal::input_handler& inp, hal::mixer& mxr) noexcept :
    m_window { wnd },
    m_input { inp }
{
}

void scene::load_level(const char* path) noexcept
{
    HAL_DEBUG_PRINT(hal::info, "Loading level ", path);

    std::ifstream file { path };

    HAL_DEBUG_VERIFY(file.is_open(), "No extra info.");
}