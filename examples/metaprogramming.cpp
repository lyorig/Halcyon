#include <halcyon/utility/metaprogramming.hpp>
#include <print>
#include <variant>

// metaprogramming.cpp:
// Showcase of Halcyon metaprogramming.

template <typename L, typename R>
constexpr inline bool eq = std::is_same_v<L, R>;

int main(int argc, char* argv[])
{
    using info = hal::function_info<decltype(main)>;

    static_assert(hal::is_correct_main_v<main>);

    static_assert(
        eq<int, info::return_type>
        && eq<int, info::args::at<0>>
        && eq<char**, info::args::at<1>>);

    info::args::wrap<std::variant> var;

    if (std::time(nullptr) % 2 == 0)
        var = 69;

    else
        var = nullptr;

    std::println("Variant holds a {}", std::holds_alternative<int>(var) ? "number" : "pointer");
}