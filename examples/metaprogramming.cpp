#include <halcyon/utility/metaprogramming.hpp>
#include <print>
#include <variant>

// metaprogramming.cpp:
// Showcase of Halcyon metaprogramming.

template <typename L, typename R>
constexpr inline bool eq = std::is_same_v<L, R>;

int main(int, char*[])
{
    using info_main  = hal::meta::func_info<decltype(main)>;
    using info_qsort = hal::meta::func_info<decltype(std::qsort)>;

    using joined = hal::meta::join<info_main::args, info_qsort::args>;
    static_assert(joined::size == info_main::args::size + info_qsort::args::size);

    using variant = joined::wrap<std::variant>;

    variant var;

    if (std::time(nullptr) % 2 == 0)
        var = 69;

    else
        var.emplace<joined::back>(nullptr);

    std::println("Variant holds a {}", std::holds_alternative<int>(var) ? "number" : "pointer");
}