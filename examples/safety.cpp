#include <halcyon/video.hpp>

int main(int, char*[])
{
    hal::context ctx;

    hal::system::video vid { ctx /* nemůže být dočasný objekt */ };
    hal::window        yes = vid /* nemůže být dočasný objekt */.make_window("Okno" /* nemůže být nullptr */, { 640, 480 });

    // [destruktor okna]
    // [destruktor systému]
    // [destruktor kontextu, konečný cleanup]

    return EXIT_SUCCESS;
}