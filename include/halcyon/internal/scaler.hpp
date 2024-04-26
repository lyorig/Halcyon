#pragma once

#include <halcyon/types/render.hpp>

// internal/scaler.hpp:
// I don't really know how to describe this class, but it's necessary.

namespace hal
{
    class scaler
    {
    public:
        using mul_t = f32;

        enum class type
        {
            width,
            height,
            multiply,
            custom
        };

        // Scale based on width.
        scaler(pixel_t val);

        // Scale based on height.
        scaler(pixel_t val, int);

        // Scale by multiplying both dimensions.
        scaler(mul_t mul);

        // Use a custom size.
        scaler(pixel_point sz);

        // Get the resulting point.
        pixel_point process(pixel_point src) const;

    private:
        union
        {
            pixel_point pt;
            mul_t       mul;
        } m_data;

        type m_type;
    };

    namespace scale
    {
        scaler width(pixel_t val);
        scaler height(pixel_t val);
        scaler mult(scaler::mul_t val);
        scaler custom(pixel_point val);
    }
}