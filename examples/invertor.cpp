#include <iostream>

#include <halcyon/image.hpp>

#include <halcyon/video/message_box.hpp>

// invertor.cpp:
// Inverts a given image.

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
        hal::message_box::build()
            .title(hal::string_from_pack("Could not load ", argv[1]))
            .message(hal::debug::last_error())
            .type(hal::message_box::type::error)();

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