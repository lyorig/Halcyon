#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>

int main(int argc, char* argv[])
{
    using enum hal::audio::property;
    lyo::parser p { argc, argv };

    hal::audio::listener l;
    hal::audio::buffer   b { hal::audio::decoder::wav(p.parse("-file=", "assets/cantina.wav")) };
    hal::audio::source   s;

    s.set<position>({
        p.parse<hal::audio::float_t>("-x=", 0),
        p.parse<hal::audio::float_t>("-y=", 0),
        p.parse<hal::audio::float_t>("-z=", 0),
    });

    s.attach(b);
    s.play();

    while (s.playing())
        ;

    return EXIT_SUCCESS;
}