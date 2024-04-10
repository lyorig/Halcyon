#pragma once

#include <string>

// clipboard.hpp:
// System clipboard handling functions.
// This is a bit goofy on some systems (at least on Wayland),
// so your mileage may vary.

namespace hal::clipboard
{
    std::string get();
    void        set(std::string_view text);
    bool        has_text();
}