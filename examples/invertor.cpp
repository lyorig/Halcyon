#include <iostream>

#include <halcyon/image.hpp>
#include <halcyon/utility/printing.hpp>

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);

    if (argc == 1) {
        std::cout << "No file specified.\n";
        return EXIT_FAILURE;
    }

    hal::image::context ctx{hal::image::init_format::png};
    hal::surface surf{ctx.load(hal::access(argv[1]))};

    if (!surf.valid()) {
        std::cout << "Could not load: " << hal::debug::last_error() << '\n';
        return EXIT_FAILURE;
    }

    const hal::pixel_point size{surf.size()};

    for (hal::pixel_point i{0, 0}; i.y < size.y; ++i.y) {
        for (i.x = 0; i.x < size.x; ++i.x) {
            auto col{surf[i]};
            col.color(-col.color());
        }
    }

    surf.save(hal::output("invert.bmp"));

    return EXIT_SUCCESS;
}