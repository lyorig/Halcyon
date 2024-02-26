#include "classes.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <lyo/stack_polymorphism.hpp>
#include <lyo/timer.hpp>
#include <variant>
#include <vector>

// A benchmark of ways to do polymorphism.
using stack   = lyo::stack_pmr<virtual_base, virtual1, virtual2, virtual3>;
using variant = std::variant<visit1, visit2, visit3>;

int sum;

lyo::f64 stack_sorted(std::size_t size)
{
    std::vector<stack> vec;
    vec.reserve(size);

    const auto third = size / 3;

    for (std::size_t i = 0; i < third; ++i)
    {
        vec.push_back(virtual1 {});
    }

    for (std::size_t i = 0; i < third; ++i)
    {
        vec.push_back(virtual2 {});
    }

    for (std::size_t i = 0; i < third; ++i)
    {
        vec.push_back(virtual3 {});
    }

    lyo::precise_timer t;

    for (const auto& val : vec)
    {
        sum += val->func();
    }

    return t();
}

lyo::f64 stack_unsorted(std::size_t size)
{
    std::vector<stack> vec(size);
    for (auto& elem : vec)
    {
        switch (std::rand() % 3)
        {
        case 0:
            elem = virtual1 {};
            continue;

        case 1:
            elem = virtual2 {};
            continue;

        case 2:
            elem = virtual3 {};
            continue;
        }
    }

    lyo::precise_timer t;

    for (const auto& val : vec)
    {
        sum += val->func();
    }

    return t();
}

lyo::f64 variant_sorted(std::size_t size)
{
    std::vector<variant> vec;
    vec.reserve(size);

    const auto third = size / 3;

    for (std::size_t i = 0; i < third; ++i)
    {
        vec.push_back(visit1 {});
    }

    for (std::size_t i = 0; i < third; ++i)
    {
        vec.push_back(visit2 {});
    }

    for (std::size_t i = 0; i < third; ++i)
    {
        vec.push_back(visit3 {});
    }

    lyo::precise_timer t;

    for (const auto& val : vec)
    {
        sum += std::visit([](auto&& x)
            { return x.func(); },
            val);
    }

    return t();
}

lyo::f64 variant_unsorted(std::size_t size)
{
    std::vector<variant> vec(size);
    for (auto& elem : vec)
    {
        switch (std::rand() % 3)
        {
        case 0:
            elem = visit1 {};
            continue;

        case 1:
            elem = visit2 {};
            continue;

        case 2:
            elem = visit3 {};
            continue;
        }
    }

    lyo::precise_timer t;

    for (const auto& val : vec)
    {
        sum += std::visit([](auto&& x)
            { return x.func(); },
            val);
    }

    return t();
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ./HalTest [iterations]\n";
        return EXIT_FAILURE;
    }

    auto size = std::atol(argv[1]);

    if (size == 0)
    {
        std::cout << "Invalid value or 0 given as size.\n";
        return EXIT_FAILURE;
    }

    size -= size % 3;

    std::cout << std::fixed << std::setprecision(8) << "Beginning with " << size << " iterations.\n";

    std::srand(std::time(nullptr));

    std::cout << "Sorted stack:\t\t" << stack_sorted(size) << " (sum = " << sum << ")\n";
    std::cout << "Unsorted stack:\t\t" << stack_unsorted(size) << " (sum = " << sum << ")\n";
    std::cout << "Sorted variant:\t\t" << variant_sorted(size) << " (sum = " << sum << ")\n";
    std::cout << "Unsorted variant:\t" << variant_unsorted(size) << " (sum = " << sum << ")\n";

    return EXIT_SUCCESS;
}