#pragma once

namespace lyo
{
    template <typename T>
    class pass_key
    {
        friend T;

        constexpr pass_key() { }
    };
} // namespace lyo