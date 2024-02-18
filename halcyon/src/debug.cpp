#include <halcyon/debug.hpp>

using namespace hal;

std::ofstream            debug::m_output { "Halcyon Debug Output.txt" };
const lyo::precise_timer debug::m_timer {};