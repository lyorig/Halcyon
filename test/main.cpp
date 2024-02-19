#include <AL++/buffer.hpp>
#include <AL++/decoder.hpp>
#include <AL++/listener.hpp>
#include <AL++/source.hpp>
#include <AL++/state.hpp>
#include <halcyon/halcyon.hpp>

int main()
{
    alpp::listener l;
    alpp::buffer   b = alpp::decoder::wav("assets/magicspear.wav");
    alpp::source   s;

    s.set<alpp::relative_source>(true);

    alpp::state::set<alpp::doppler_factor>(2.0);

    s.set<alpp::looping>(true);
    s.attach(b);
    s.play();

    char          ch;
    alpp::float_t sos { alpp::state::get<alpp::speed_of_sound>() }; // speed of sound

    while ((ch = std::getchar()) != 'q')
    {
        constexpr alpp::float_t incr { 100.0 };

        switch (ch)
        {
        case 'w':
            sos += incr;
            l.set<alpp::velocity>({ 0, sos, 0 });
            break;
        case 's':
            sos -= incr;
            l.set<alpp::velocity>({ 0, sos, 0 });
            break;
        case 'g':
            HAL_PRINT(l.get<alpp::velocity>());
        default:
            break;
        }
    }

    return 0;
}