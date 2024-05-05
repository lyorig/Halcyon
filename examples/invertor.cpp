#include <iostream>

#include <halcyon/image.hpp>

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [image path]\n";
        return EXIT_FAILURE;
    }

    hal::image::context ctx { hal::image::init_format::png };
    hal::surface        surf { ctx.load(hal::access(argv[1])) };

    if (!surf.valid())
    {
        std::cout << "Could not load file: " << hal::debug::last_error() << '\n';
        return EXIT_FAILURE;
    }

    const hal::pixel_point size { surf.size() };

    for (hal::pixel_point i { 0, 0 }; i.y < size.y; ++i.y)
    {
        for (i.x = 0; i.x < size.x; ++i.x)
        {
            auto col { surf[i] };
            col.color(-col.color());
        }
    }

    ctx.save(surf, hal::image::save_format::png, hal::output("invert.png"));

    return EXIT_SUCCESS;
}