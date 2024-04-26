#include <halcyon/internal/scaler.hpp>

using namespace hal;

scaler::scaler(pixel_t val, HAL_TAG_NAME(scale_width))
    : m_data { .val = val }
    , m_type { type::width }
{
}

scaler::scaler(pixel_t val, HAL_TAG_NAME(scale_height))
    : m_data { .val = val }
    , m_type { type::height }
{
}

scaler::scaler(mul_t mul)
    : m_data { .mul = mul }
    , m_type { type::multiply }
{
}

point<scaler::val_t> scaler::operator()(point<val_t> src) const
{
    switch (m_type)
    {
        using enum type;

    case width:
        return src * (static_cast<mul_t>(m_data.val) / src.x);

    case height:
        return src * (static_cast<mul_t>(m_data.val) / src.y);

    case multiply:
        return src * m_data.mul;
    }
}

scaler scale::width(scaler::val_t val)
{
    return { val, tag::scale_width };
}

scaler scale::height(scaler::val_t val)
{
    return { val, tag::scale_height };
}

scaler scale::mul(scaler::mul_t val)
{
    return { val };
}