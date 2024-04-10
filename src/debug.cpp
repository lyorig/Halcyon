#include <halcyon/debug.hpp>

#ifdef HAL_DEBUG_ENABLED

using namespace hal;

    #ifdef HAL_DEBUG_ADVANCED
std::ofstream            debug::m_output { "Halcyon Debug Output.txt" };
const lyo::precise_timer debug::m_timer {};
    #endif

std::string_view debug::last_error()
{
    const char* err { ::SDL_GetError() };

    return lyo::is_c_string_empty(err) ? "[no SDL error]" : err;
}

#endif