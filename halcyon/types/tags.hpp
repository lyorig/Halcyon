#pragma once

#include <lyo/types.hpp>

/* tags.hpp:
   For internal use only. This file is required to fix
   a circular dependency in point.hpp and rectangle.hpp. */

namespace hal
{
    TAG_TYPE(as_size);

    TAG_TYPE(infinite_loop);
}