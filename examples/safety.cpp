#include <halcyon/event.hpp>
#include <halcyon/video.hpp>

int main(int argc, char* argv[])
{
    // A simple way to check whether your main function is correct.
    static_assert(hal::is_correct_main_v<main>);

    // SDL:
    // Are the necessary systems initialized? Are the parameters correct?
    SDL_Surface* ptr = ::IMG_Load(nullptr);

    ptr = nullptr; // oops, memory leak!
    ++ptr;         // what are you doing??

    // Halcyon:
    hal::image::context img { hal::image::init_format::jpg }; // can't initialize without a context

    hal::surface surf = img.load(hal::access("[file name here]"));
}