#pragma once

namespace lyo
{
    template <typename T>
    class pass_key
    {
      private:

        friend T;

        pass_key() {}
    };
}  // namespace lyo