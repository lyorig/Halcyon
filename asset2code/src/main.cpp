#include <halcyon/cleanup.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/other/clipboard.hpp>
#include <halcyon/other/printing.hpp>
#include <halcyon/surface.hpp>
#include <lyo/timer.hpp>
#include <sstream>

#include <iostream>

constexpr char helptext[] {
    "This utility takes an image file and converts it into an initializer list of Halcyon colors.\n"
    "Usage: asset2code [path]\n"
};

std::ostream& operator<<(std::ostream& str, const hal::color& c)
{
    str << '{' << hal::to_printable_int(c.r) << ','
        << hal::to_printable_int(c.g) << ','
        << hal::to_printable_int(c.b) << ','
        << hal::to_printable_int(c.a) << '}';
    return str;
}

int main(int argc, char* argv[])
{
    if (argc == 1) // Is there an argument?
    {
        std::cout << helptext;
        return EXIT_FAILURE;
    }

    lyo::precise_timer t;
    hal::cleanup       c { hal::system::video };

    if (const auto surf { hal::image_loader::load(argv[1]) }; surf.exists())
    {
        std::stringstream s;
        s << '{';

        const auto size = surf.size();
        for (hal::pixel_point pt { 0, 0 }; pt.y != size.y; ++pt.y, pt.x = 0)
        {
            for (; pt.x != size.x; ++pt.x)
            {
                s << surf[pt] << ',';
            }
        }

        s << "};";

        const auto str = s.str();
        hal::clipboard::set(str);
        std::cout << "Done in " << t() << "s, copied " << str.size() << " bytes.\n";
    }

    else
    {
        std::cout << "Could not load \"" << argv[1] << "\".\n";
        return EXIT_FAILURE;
    }
}