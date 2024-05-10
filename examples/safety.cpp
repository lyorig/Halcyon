#include <halcyon/event.hpp>
#include <halcyon/video.hpp>

int main(int argc, char* argv[])
{
    hal::context ctx;

    // Nelze využít systém jednorázově pro tvorbu objektů.
    // hal::window no = hal::system::video { ctx }.make_window();

    // Takto je to správně. Systém se takto vypne až po zničení okna.
    hal::system::video vid { ctx };
    hal::window        yes = vid.make_window("Okno", { 640, 480 });
}