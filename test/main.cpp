#include "classes.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <lyo/stack_polymorphism.hpp>
#include <lyo/timer.hpp>
#include <variant>
#include <vector>

// A benchmark of ways to do polymorphism.
using stack   = lyo::stack_pmr<virtual_base, virtual1, virtual2, virtual3, virtual4, virtual5, virtual6, virtual7, virtual8, virtual9>;
using variant = std::variant<visit1, visit2, visit3, visit4, visit5, visit6, visit7, visit8, visit9>;

int sum;

lyo::f64 stack_sorted(std::size_t size)
{
    std::vector<stack> vec;
    vec.reserve(size);

    const auto ninth = size / 9;

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual1 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual2 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual3 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual4 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual5 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual6 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual7 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual8 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(virtual9 {});
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
        switch (std::rand() % 9)
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

        case 3:
            elem = virtual4 {};
            continue;

        case 4:
            elem = virtual5 {};
            continue;

        case 5:
            elem = virtual6 {};
            continue;

        case 6:
            elem = virtual7 {};
            continue;

        case 7:
            elem = virtual8 {};
            continue;

        case 8:
            elem = virtual9 {};
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

    const auto ninth = size / 9;

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit1 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit2 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit3 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit4 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit5 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit6 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit7 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit8 {});
    }

    for (std::size_t i = 0; i < ninth; ++i)
    {
        vec.push_back(visit9 {});
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
        switch (std::rand() % 9)
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

        case 3:
            elem = visit4 {};
            continue;

        case 4:
            elem = visit5 {};
            continue;

        case 5:
            elem = visit6 {};
            continue;

        case 6:
            elem = visit7 {};
            continue;

        case 7:
            elem = visit8 {};
            continue;

        case 8:
            elem = visit9 {};
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

    size -= size % 9;

    std::cout << std::fixed << std::setprecision(8) << "Beginning with " << size << " iterations.\n";

    std::srand(std::time(nullptr));

    const lyo::f64 times[4] {
        stack_sorted(size), stack_unsorted(size), variant_sorted(size), variant_unsorted(size)
    };

    const lyo::f64 fastest = *std::min_element(std::begin(times), std::end(times));

    std::cout << "Sorted stack\tUnsorted stack\tSorted variant\tUnsorted variant\n";
    for (auto t : times)
        std::cout << t << "\t\t";

    std::cout << '\n'
              << std::setprecision(4);

    for (auto t : times)
        std::cout << t / fastest << "x\t\t\t";

    std::cout << '\n';

    return EXIT_SUCCESS;
}