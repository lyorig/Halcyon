#pragma once

#include <quest/ents/modifiers.hpp>
#include <quest/ents/prefabs.hpp>

namespace quest::ent
{
    class player : public mod::non_unique, pfb::character
    {
    };
}