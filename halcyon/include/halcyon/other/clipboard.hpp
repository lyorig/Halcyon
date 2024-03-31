#pragma once

#include <string>

// clipboard.hpp:
// System clipboard handling functions.
// This is a bit goofy on some systems (at least on Wayland),
// so your mileage may vary.

namespace hal
{
    std::string clipboard();
    void        clipboard(std::string_view text);
}