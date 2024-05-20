#include <halcyon/video.hpp>

int main(int, char*[])
{
    // SDL enrolls its own main function on Windows, which is a common source of headaches.
    // A compile-time check is provided to ensure your program passes this part of the cross-platform check.
    static_assert(hal::meta::is_correct_main<main>);

    // Halcyon employs some safeguards to make sure you don't use uninitialized systems.
    // For example, here are a few mistakes you could make in SDL:

    SDL_Window* sdl_wnd { ::SDL_CreateWindow(nullptr, // nullptr supplied as string.
        0, 0, 640, 480,
        69420) }; // Random integer passed as bitmask.

    // Wait, did we even initialize the library?!
    // (in this case, the function checks and does this for us, but such altruism is in no way guaranteed)

    SDL_Renderer* sdl_rnd { ::SDL_CreateRenderer(sdl_wnd, -1, 123456789) }; // Same bitmask issue as before.

    // The file path describes what this line does pretty well.
    SDL_Texture* sdl_tex { ::SDL_CreateTextureFromSurface(sdl_rnd, ::IMG_Load("memory/leak.png")) };

    ::SDL_DestroyTexture(sdl_tex); // You could very much forget this and leak memory.
    ::SDL_DestroyRenderer(sdl_rnd);
    ::SDL_DestroyWindow(sdl_wnd);

    // Now, here's how all this is improved in Halcyon.

    // A context must first be created - you cannot do much without one.
    hal::context ctx;

    // Bitmasks are replaced with initializer lists of scoped enums.
    hal::image::context img { hal::image::init_format::jpg };

    // Subsystem initialization.
    // "ctx" is taken by lvalue reference, it cannot be a temporary.
    hal::system::video vid { ctx };

    // "vid" only makes this function available if it's an lvalue.
    // Thus, subsystem initialization is guaranteed at this point, eliminating possible errors.
    hal::window wnd = vid.make_window("Okno", // std::string_view cannot be constructed from nullptr.
        { 640, 480 });

    // Likewise for "wnd". Temporaries cannot create objects that rely on them.
    hal::renderer rnd { wnd.make_renderer() };

    // Image loading is only possible with an image context.
    // Oops, PNG support isn't loaded! If this fails at any stage, Halcyon Debug will let you know.
    hal::texture tex { rnd.make_texture(img.load("assets/file.png")) };

    // Deallocation + deinitialization is taken care of, so you can focus on the important part - coding!
    // You also don't have to work with pointers at all. But if your favorite SDL function isn't implemented,
    // all relevant objects provide a get() method which, just like unique_ptr, returns the underlying pointer.

    // Other things that improve the experience:
    // - optional debugging that checks basically everything
    // - type ecosystem (point, rectangle, color...)
    // - builder patterns for creating/doing stuff with multiple paramaters (message boxes, rendering)

    return EXIT_SUCCESS;
}