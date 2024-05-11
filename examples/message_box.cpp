#include <ctime>

#include <halcyon/video/message_box.hpp>

constexpr std::string_view
    titles[] {
        "Hello there...",
        "Womp womp.",
        "All your base are belong to us!",
        "<< Surprise Popup >>"
    },
    messages[] {
        "What a thrill",
        "rm -rf /",
        "Shawty like a melody in my head",
        "Le fishe au chocolat",
        "Aeaeaeaeaeaeeaeeaeaeaeaeaeea",
        "This post was made by M14 gang"
    };

std::string_view random(std::span<const std::string_view> span)
{
    return span[std::time(nullptr) % span.size()];
}

int main(int, char*[])
{
    using enum hal::message_box::type;

    auto msgb = hal::message_box::builder().buttons({ "Yes", "No", "Maybe" });

    for (auto type : { info, warning, error })
        HAL_PRINT(
            "User pressed button #",
            hal::to_printable_int(
                msgb.type(type)
                    .title(random(titles))
                    .message(random(messages))()));

    return EXIT_SUCCESS;
}