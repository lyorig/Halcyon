#include <halcyon/cleanup.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/other/clipboard.hpp>
#include <halcyon/other/printing.hpp>
#include <halcyon/surface.hpp>
#include <lyo/timer.hpp>

#include <iostream>

std::ostream& operator<<(std::ostream& str, const hal::color& c)
{
    str << '{' << hal::to_printable_int(c.r) << ','
        << hal::to_printable_int(c.g) << ','
        << hal::to_printable_int(c.b) << ','
        << hal::to_printable_int(c.a) << '}';
    return str;
}

void put_color(std::string& str, hal::color c)
{
    char buffer[20];
    std::snprintf(buffer, sizeof(buffer), "{%u,%u,%u,%u},", unsigned(c.r), unsigned(c.g), unsigned(c.b), unsigned(c.a));
    str += buffer;
}

std::size_t alloc_size(hal::pixel_point sz)
{
    constexpr std::size_t max_chars_per_color { 19 };
    return sz.x * sz.y * max_chars_per_color + 3; // outer braces and null terminator
}

int main(int argc, char* argv[])
{
    if (argc != 3) // Is there an argument?
    {
        constexpr char helptext[] {
            "This utility takes an image file and converts it into an initializer list of Halcyon colors.\n"
            "Usage: asset2code [-r/-c] [path]\n"
        };

        std::cout << helptext;
        return EXIT_FAILURE;
    }

    lyo::precise_timer t;
    hal::cleanup       c { hal::system::video };

    if (const auto surf { hal::image_loader::load(argv[2]) }; surf.exists())
    {
        const auto size = surf.size();

        std::string s { '{' };
        s.reserve(alloc_size(size));

        if (lyo::streq(argv[1], "-r")) // Row major
        {
            for (hal::pixel_point pt { 0, 0 }; pt.y != size.y; ++pt.y, pt.x = 0)
            {
                for (; pt.x != size.x; ++pt.x)
                {
                    put_color(s, surf[pt]);
                }
            }
        }

        else if (lyo::streq(argv[1], "-c")) // Column major
        {
            for (hal::pixel_point pt { 0, 0 }; pt.x != size.x; ++pt.x, pt.y = 0)
            {
                for (; pt.y != size.y; ++pt.y)
                {
                    put_color(s, surf[pt]);
                }
            }
        }

        else
        {
            std::cout << "Unknown ordering option \"" << argv[1] << "\".\n";
            return EXIT_FAILURE;
        }

        s.push_back('}');
        hal::clipboard::set(s);
        std::cout << "Done in " << t() << "s, copied " << s.size() << " bytes.\n";
    }

    else
    {
        std::cout << "Could not load \"" << argv[2] << "\".\n";
        return EXIT_FAILURE;
    }
}