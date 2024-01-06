#include <ecs/component_manager.hpp>
#include <ecs/entity.hpp>
#include <ecs/scene.hpp>
#include <halcyon/halcyon.hpp>
#include <iostream>
#include <lyo/argparse.hpp>
#include <vector>

using sz = std::size_t;

template <typename I>
I random_element(I begin, I end)
{
    std::advance(begin, std::rand() % std::distance(begin, end));
    return begin;
}

constexpr int  draw_iters { 32 };
constexpr char string[] { "abcdef1234" };
constexpr char help_text[] {
    "Haltest, by lyorig.\n"
    "Options:\n"
    "\t-h\t- Show this message.\n"
    "\t-s\t- Measure surface performance.\n"
    "\t-t\t- Measure texture performance.\n"
    "\t-ecs\t- Test the ECS.\n"
    "\t-iter=[num]\t- The amount of iterations (default 100).\n"
};

struct holder
{
    LYO_MAYBE_EMPTY hal::engine eng;
    LYO_MAYBE_EMPTY hal::video vid { eng };
    LYO_MAYBE_EMPTY hal::ttf_engine ttf { vid };

    hal::input_handler inp { eng };
    hal::window        wnd { vid, "Renderer proxy", {}, { 100, 100 }, {} };
    hal::renderer      rnd { wnd, { hal::renderer::accelerated } };

    const hal::font fnt { ttf.load("assets/m5x7.ttf", 144) };
};

void surface_drawing(holder& hld)
{
    hal::surface res { hld.vid, { 1024, 768 } };

    for (int i = 0; i < draw_iters; ++i)
        hal::blit(hld.fnt.render(string))(res);

    hal::texture tes { hld.rnd, res };
}

void texture_drawing(holder& hld)
{
    hal::target_texture tex { hld.rnd, { 1024, 768 } };
    hal::target_lock    tl { hld.rnd, tex };

    for (int i = 0; i < draw_iters; ++i)
    {
        const hal::texture dt { hld.rnd, hld.fnt.render(string) };
        hal::draw { dt }(hld.rnd);
    }
}

void sv_fuzz()
{
    std::cout << "\nStarting static_vector fuzzer.\n";

    struct test
    {
        ~test()
        {
            std::cout << "D ";
        }
    };

    lyo::static_vector<test, 10> ts;
    assert(ts.size() == 0);

    std::cout << "[Filling up to capacity]";
    for (sz i { 0 }; i < ts.capacity(); ++i)
    {
        assert(ts.size() == i);
        ts.emplace_back();
    }

    std::cout << "[Cleaning]";
    ts.clear();
    assert(ts.size() == 0);

    std::cout << "\n[Resizing to capacity]";
    ts.resize(ts.capacity(), lyo::no_init);
    assert(ts.size() == ts.capacity());

    std::cout << "\n[Erasing three elements]";
    ts.erase(ts.begin() + 2, ts.begin() + 5);
    assert(ts.size() == ts.capacity() - 3);

    std::cout << "\n[Erasing a single element]";
    ts.erase(ts.begin() + 5);
    assert(ts.size() == ts.capacity() - 4);

    std::cout << "\n[Destructor]";
}

void cmgr_fuzz(sz iters)
{
    std::cout << "\nStarting component manager fuzzer.\n";
    using ECS::comp::info;

    using comp_mgr = ECS::static_component_manager<
        info<hal::pixel_size, 10>,
        info<hal::coord, 5>,
        info<int, 50>>;

    comp_mgr cm;

    // Part one: Allocation and reallocation.
    for (sz i { 0 }; i < iters; ++i)
    {
        // using lim = std::numeric_limits<hal::pixel_type>;
        using tp = int;

        const tp x { std::rand() };

        const auto id = cm.add<tp>(x);
        assert(x == cm.get<tp>(id));
        cm.remove<tp>(id);
    }

    std::cout << "Component manager fuzzer ended.\n";
}

void ecs_test(sz iters [[maybe_unused]])
{
    std::cout << "Commencing ECS test.\n";

    using t1 = hal::pixel_size;
    using t2 = hal::pixel_area;

    using ECS::comp::info;
    using scm = ECS::static_component_manager<
        info<t1, 1000>,
        info<t2, 1000>>;
    using sem = ECS::dynamic_scene<scm, ECS::static_entity>;

    lyo::precise_timer tmr;

    if (true)
    {
        sem manager;

        // Part one: Entity allocation.
        std::vector<sem::entity::ID> ids;

        for (sz i { 0 }; i < iters; ++i)
        {
            ids.push_back(manager.spawn<t1, t2>());
            // std::cout << "Spawned ID " << ids.back() << '\n';
        }

        for (sz i { 0 }; i < iters; ++i)
        {
            const auto iter = random_element(ids.begin(), ids.end());

            // std::cout << "Killed ID " << *iter << '\n';
            manager.kill(manager.find(*iter));
            ids.erase(iter);
        }

        // Part two: Guaranteed reallocation.
        for (sz i { 0 }; i < iters; ++i)
        {
            ids.push_back(manager.spawn<t1, t2>());
            assert(manager.ents() == 1);

            const auto iter = random_element(ids.begin(), ids.end());

            manager.kill(manager.find(*iter));
            ids.erase(iter);
        }
    }

    sv_fuzz();
    cmgr_fuzz(iters);

    std::cout << "Test concluded, having taken " << tmr() << "s.\n";
}

int main(int argc, char* argv[])
{
    std::srand(std::time(nullptr));
    std::ios_base::sync_with_stdio(false);

    lyo::parser p { argc, argv };

    const auto iter = p.parse<sz>("-iter=", 100);

    if (p.has("-h"))
    {
        std::cout << help_text << 'n';
        return EXIT_SUCCESS;
    }

    const bool arg_t { p.has("-t") }, arg_s { p.has("-s") }, arg_ecs { p.has("-ecs") };

    if (!(arg_t || arg_s || arg_ecs))
    {
        std::cout << "No measuring options specified. Exiting.\n";
        return EXIT_FAILURE;
    }

    if (arg_ecs)
        ecs_test(iter);

    if (arg_t || arg_s)
    {
        holder   h;
        lyo::f64 surface_result, texture_result;

        lyo::precise_timer tmr;

        if (arg_s)
        {
            for (sz i { 0 }; i < iter; ++i)
                surface_drawing(h);

            surface_result = tmr();
        }

        tmr.reset();

        if (arg_t)
        {
            for (sz i { 0 }; i < iter; ++i)
                texture_drawing(h);

            texture_result = tmr();
        }

        if (arg_s)
            std::cout << "Surface drawing took " << surface_result << "s\n";

        if (arg_t)
            std::cout << "Texture drawing took " << texture_result << "s\n";

        if (arg_t && arg_s)
            std::cout << "Result: Texture drawing is " << surface_result / texture_result << "x faster\n";
    }

    return EXIT_SUCCESS;
}