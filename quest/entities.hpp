#pragma once

#include "constants.hpp"
#include <halcyon/renderer.hpp>
#include <string_view>

namespace quest
{
    template <typename T>
    concept updateable = requires(T x) { x.update(); };

    template <typename T>
    concept drawable = requires(T x, hal::renderer& rnd) { x.draw(rnd); };

    // Entity base class. Contains common functionality.
    class entity
    {
    public:
        using ID = lyo::u32;

        constexpr entity(ID id)
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

    class player : public entity
    {
    public:
        using entity::entity;

        constexpr std::string_view name() const
        {
            return "Player";
        }
    };

    class bullet_shell : public entity
    {
    public:
        using entity::entity;

        constexpr std::string_view name() const
        {
            return "Bullet shell";
        }
    };

    class light : public entity
    {
    public:
        using entity::entity;

        constexpr std::string_view name() const
        {
            return "Light";
        }
    };
}