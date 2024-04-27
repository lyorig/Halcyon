#include <halcyon/internal/scaler.hpp>

using namespace hal;

scaler::scaler(pixel_t val, HAL_TAG_NAME(scale_width))
    : m_type { type::width }
    , m_data { .val = val }
{
}

scaler::scaler(pixel_t val, HAL_TAG_NAME(scale_height))
    : m_type { type::height }
    , m_data { .val = val }
{
}

scaler::scaler(mul_t mul)
    : m_type { type::multiply }
    , m_data { .mul = mul }
{
}

point<scaler::val_t> scaler::operator()(point<val_t> src) const
{
    switch (m_type)
    {
        using enum type;

    case width:
        return {
            m_data.val,
            static_cast<val_t>(src.y * (static_cast<mul_t>(m_data.val) / src.x))
        };

    case height:
        return {
            static_cast<val_t>(src.x * (static_cast<mul_t>(m_data.val) / src.y)),
            m_data.val
        };

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