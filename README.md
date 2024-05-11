# About Halcyon
A slim, performant SDL wrapper written in modern C++.  
Performance is the main priority here, but work has also been put into making sure there is some level of safety as well.  
Namespaces are not cluttered, everything is neatly organized - proper C++ design, plain and simple.  

# Installation
Halcyon is included into your project via CMake.
1. Clone the repository into your project directory.
> [!CAUTION]
> Do a shallow clone, as there was a time when actual MP3 and WAV files were stored here. I'll get around to removing these commits from the history eventually.
2. In your CMakeLists.txt, `include(halcyon/config.cmake)`. This will ensure all necessary packages are installed.
3. Several variables now exist: `HAL_INCLUDE_DIRS`, `HALCYON_SOURCES` and `HALCYON_LIBRARIES`. Use these with your project and you're good to go.

# Usage
Halcyon wraps SDL using the concepts of *contexts* and *systems*.
- **Context:** A "top-level" class that manages a library's (de)initialization.
- **System:** Initializes specific context functionality, if such separation is necessary.
> [!TIP]
> All of the above are empty classes. You are encouraged to use them with `[[no_unique_address]]` (also defined as `HAL_NO_SIZE`).