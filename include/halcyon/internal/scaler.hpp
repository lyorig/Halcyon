#pragma once

#include <halcyon/types/render.hpp>

// internal/scaler.hpp:
// A way to modify pixel sizes in-place.

namespace hal
{
    HAL_TAG(scale_width);
    HAL_TAG(scale_height);

    class scaler
    {
    public:
        using val_t = pixel_t;
        using mul_t = f32;

        enum class type
        {
            width,
            height,
            multiply
        };

        // Scale based on width.
        scaler(val_t val, HAL_TAG_NAME(scale_width));

        // Scale based on height.
        scaler(val_t val, HAL_TAG_NAME(scale_height));

        // Scale by multiplying both dimensions.
        scaler(mul_t mul);

        // Get the resulting point.
        point<val_t> operator()(point<val_t> src) const;

    private:
        union
        {
            val_t val;
            mul_t mul;
        } m_data;

        type m_type;
    };

    namespace scale
    {
        scaler width(scaler::val_t val);
        scaler height(scaler::val_t val);
        scaler mul(scaler::mul_t mul);
    }
}