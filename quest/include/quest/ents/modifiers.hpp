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

        constexpr non_unique(ID id)
            : m_id { id }
        {
        }

        constexpr ID id() const
        {
            return m_id;
        }

    private:
        ID m_id;
    };
}