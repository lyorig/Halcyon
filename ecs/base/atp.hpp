#pragma once

#include <lyo/static_vector.hpp>

namespace ECS
{
    template <std::size_t Size>
    struct array_template_proxy
    {
        template <typename T>
        using type = lyo::static_vector<T, Size>;
    };
}