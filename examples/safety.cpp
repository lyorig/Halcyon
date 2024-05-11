#include <halcyon/events.hpp>
#include <halcyon/video.hpp>

int main(int, char*[])
{
    hal::context ctx;

    // Nelze využít systém jednorázově pro tvorbu objektů.
    // hal::window no = hal::system::video { ctx }.make_window();

    // Takto je to správně. Systém se takto vypne až po zničení okna.
    hal::system::video vid { ctx };
    hal::window        yes = vid.make_window("Okno", { 640, 480 });

    // [destruktor okna, zničí se]
    // [destruktor systému, vypne se]
    // [destruktor kontextu, konečný cleanup]

    return EXIT_SUCCESS;
}