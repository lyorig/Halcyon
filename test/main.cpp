#include <halcyon/halcyon.hpp>

int main(int argc, char* argv[])
{
    hal::audio::listener l;
    hal::audio::source   s;
    hal::audio::buffer   b = hal::audio::decoder::wav("assets/cantina.wav");

    s.set<hal::audio::buffer_id>(b.id());

    s.play();

    while (s.playing())
        ;

    return EXIT_SUCCESS;
}