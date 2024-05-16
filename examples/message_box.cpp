#include <ctime>

#include <halcyon/utility/strutil.hpp>
#include <halcyon/video/message_box.hpp>

int main(int argc, char* argv[])
{
    if (argc > 1 && hal::streq(argv[1], "-c"))
        HAL_PANIC("You asked for it");

    using enum hal::message_box::type;
    using namespace hal::palette;

    // Vytvoření skrz builder
    auto msgb = hal::message_box::builder()
                    .buttons({ "1", "2", "3", "4", "5", "6", "7" })
                    .colors(blue, red, red, orange, yellow)
                    .title("Hello from Halcyon!")
                    .body("Nothing more to say.");

    for (auto type : { info, warning, error })
    {
        const auto ret = msgb.type(type)();

        HAL_PRINT("User pressed button ", hal::to_printable_int(ret));
    }

    return EXIT_SUCCESS;
}