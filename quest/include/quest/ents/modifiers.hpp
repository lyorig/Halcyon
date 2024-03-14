#pragma once

#include <lyo/types.hpp>

namespace quest::ent::mod
{
    // Singleton entity base class. Inheritance is required for inclusion in
    // the singleton entity manager.
    class unique
    {
    };

    // Entity base class. Inheritance is required for inclusion in the entity
    // manager.
    class non_unique
    {
    public:
        using ID = lyo::u32;

        non_unique(ID id);

        ID id() const;

    private:
        ID m_id;
    };
}