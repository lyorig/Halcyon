#pragma once

#include <lyo/types.hpp>

// Entity modifier namespace.
namespace quest::ent::mod
{
    // Singleton entity base class. Inheritance is required for inclusion in
    // the singleton entity manager.
    struct unique
    {
    };

    // Entity base class. Inheritance is required for inclusion in the entity
    // manager.
    struct non_unique
    {
    public:
        using ID = lyo::u32;

        non_unique(ID id);

        ID id() const;

    private:
        ID m_id;
    };
}