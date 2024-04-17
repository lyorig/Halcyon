#include <halcyon/utility/timer.hpp>

using namespace hal;

timer::timer()
    : m_epoch { clock::now() }
{
}

void timer::reset()
{
    m_epoch = clock::now();
}

f64 timer::operator()() const
{
    return std::chrono::duration<f64> { clock::now() - m_epoch }.count();
}