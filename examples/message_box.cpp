#include <halcyon/utility/strutil.hpp>
#include <halcyon/video/message_box.hpp>

// message_box.cpp:
// Examples of using message boxes in Halcyon.

int main(int, char*[])
{
    static_assert(hal::meta::is_correct_main<main>);

    using enum hal::message_box::type;
    using namespace hal::palette;

    // Creation via builder.
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

    // Creation via the simpler show() function.
    hal::message_box::show(hal::message_box::type::info, "Hello", "World!");

    return EXIT_SUCCESS;
}