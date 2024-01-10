#pragma once

#include <lyoSTL/static_vector.hpp>

namespace lyo
{
    namespace ecs
    {
        template <std::size_t Size>
        struct array_template_proxy
        {
            template <typename T>
            using type = lyo::static_vector<T, Size>;
        };
    }
}