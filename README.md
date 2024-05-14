# About Halcyon
Halcyon is a slim, performant SDL wrapper written in modern C++.  
Performance is the main priority here, but work has also been put into making sure there is some level of safety as well.  
Namespaces are not cluttered, everything is neatly organized - proper C++ design, plain and simple.  

# Installation
Halcyon is included into your project via CMake.
1. Clone the repository into your project directory.
> [!CAUTION]
Do a shallow clone, as there was a time when actual MP3 and WAV files were stored here. I'll get around to removing these commits from the history eventually.
2. In your CMakeLists.txt, `include(halcyon/config.cmake)`. This will ensure all necessary packages[^1] are installed.
3. Several variables now exist: `HAL_INCLUDE_DIRS`, `HALCYON_SOURCES` and `HALCYON_LIBRARIES`. Use these with your project and you're good to go.

# Usage
Halcyon wraps SDL with several concepts:
- **Context:** A "top-level" class that manages a library's (de)initialization.
- **Proxy**: Provides access to specific context functionality.
- **System:** Inherits from a proxy and manages (de)initialization of its functionality.
> [!TIP]
These are empty classes. You are encouraged to use them with `[[no_unique_address]]` (also defined as `HAL_NO_SIZE`).

Both of the above also act as factories for classes that depend on their initialization. Such member functions are usually called `make_(classname)`, with some exceptions, where a more fitting name was chosen.

[^1]: Currently SDL2, SDL2_image, and SDL2_ttf.

# Example program
This library is still under heavy developement; some namespaces etc. might not be up-to-date, but the structure should remain the same.
```
#include <halcyon/video.hpp>
#include <halcyon/events.hpp>

int main(int argc, char* argv[]) {
  static_assert(hal::meta::is_correct_main<decltype(main)>);

  hal::context       ctx;
  hal::system::video vid{ctx};

  hal::window   wnd{vid.make_window("Example", {640, 480})};
  hal::renderer rnd{wnd.make_renderer()};

  hal::event::handler evt{vid.events};

  while (true) {
    while (evt.poll()) {
      if (evt.event_type() == hal::event::type::quit_requested)
        return EXIT_SUCCESS; // Normal termination.
    }

    rnd.present();
  }

  return EXIT_FAILURE; // Abnormal termination.
}
```

# Debugging
Halcyon provides several macro functions for easing your debugging experience.  
"Vital" variants preserve the condition in case debugging is disabled. Use when your condition has side effects.
- `HAL_PRINT` - print a variadic amount of arguments (must support std::ostream::operator<<).
- `HAL_ASSERT[_VITAL]` - Assert that a condition is true, panic if not.
- `HAL_WARN_IF[_VITAL]` - Print a warning message if a condition is true.
- `HAL_PANIC` - Print where the panic occurred along with variadic user-supplied information. Exits by default.

Halcyon's debugging facilities are configured via several macros.  
If NDEBUG is not defined, debugging is implicitly enabled in advanced mode.
- `HAL_DEBUG_ENABLED`: Enables all debugging macros.
- `HAL_DEBUG_ADVANCED`: Enables time logging, and outputs to an additional file. This adds static variables to your program.
- `HAL_NO_EXIT_ON_PANIC`: Disables exit-on-panic behavior. Useful for when you want to roll your own error handling, but still want to trace what went wrong.
