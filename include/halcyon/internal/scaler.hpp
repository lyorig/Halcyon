#pragma once

#include <halcyon/types/point.hpp>

// internal/scaler.hpp:
// I don't really know how to describe this class, but it's necessary.

namespace hal
{
    enum class scale_type
    {
        width,
        height,
        multiply,
        custom
    };

    template <arithmetic T>
    class scaler
    {
    public:
        scale_type type;

        point<T> process(const point<T>& src) const
        {
            switch (type)
            {
                using enum scale_type;

            case width:
                return src * (data.pt.x / src.x);

            case height:
                return src * (data.pt.y / src.y);

            case multiply:
                return src * data.mul;

            case custom:
                return data.pt;
            }
        }

        union
        {
            point<T> pt;
            f32      mul;
        } data;
    };

    namespace scale
    {
        template <arithmetic T>
        scaler<T> width(T val)
        {
            return { scale_type::width, point<T> { .x = val } };
        }

        template <arithmetic T>
        scaler<T> height(T val)
        {
            return { scale_type::height, point<T> { .y = val } };
        }

        template <arithmetic T>
        scaler<T> mul(f32 val)
        {
            return { scale_type::multiply, val };
        }

        template <arithmetic T>
        scaler<T> custom(const point<T>& val)
        {
            return { scale_type::custom, val };
        }
    }
}