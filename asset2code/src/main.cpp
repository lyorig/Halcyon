#include <halcyon/cleanup.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/other/clipboard.hpp>
#include <halcyon/other/printing.hpp>
#include <halcyon/surface.hpp>

#include <iostream>

std::ostream& operator<<(const hal::color& c, std::ostream& str)
{
    str << '{' << c.r << ',' << c.g << ',' << c.b << ',' << c.a << '}';
    return str;
}

int main(int argc, char* argv[])
{
    if (argc == 1) // Is there an argument?
    {
        std::cout << "Usage: asset2code [filename]\n";
        return EXIT_FAILURE;
    }

    hal::cleanup c { hal::system::video };

    // Okay, so we have a filename.
    const auto surf { hal::image_loader::load(argv[1]) };
    if (!surf.exists())
    {
        std::cout << "Could not load \"" << argv[1] << "\".\n";
        return EXIT_FAILURE;
    }

    // We have a working surface.
    std::cout << "Received asset of size " << surf.size() << '\n'
              << "Current clipboard: " << hal::clipboard::get() << '\n';

    hal::clipboard::set(argv[1]);
}