#include <halcyon/internal/scaler.hpp>

using namespace hal;

scaler::scaler(pixel_t val)
    : m_data { .pt = { .x = val } }
    , m_type { type::width }
{
}

scaler::scaler(pixel_t val, int)
    : m_data { .pt = { .x = val } }
    , m_type { type::height }
{
}

scaler::scaler(mul_t mul)
    : m_data { .mul = mul }
    , m_type { type::multiply }
{
}

scaler::scaler(pixel_point sz)
    : m_data { .pt = sz }
    , m_type { type::custom }
{
}
pixel_point scaler::process(pixel_point src) const
{
    switch (m_type)
    {
        using enum type;

    case width:
        return src * (static_cast<mul_t>(m_data.pt.x) / src.x);

    case height:
        return src * (static_cast<mul_t>(m_data.pt.x) / src.y);

    case multiply:
        return src * m_data.mul;

    case custom:
        return m_data.pt;
    }
}

scaler scale::width(pixel_t val)
{
    return { val };
}

scaler scale::height(pixel_t val)
{
    return { val, 69420 };
}

scaler scale::mult(scaler::mul_t val)
{
    return { val };
}

scaler scale::custom(pixel_point val)
{
    return { val };
}