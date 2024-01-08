#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <lyoECS/base/component.hpp>

// component_manager.hpp:
// HalECS' component manager implementation.
// Honestly, the static component manager is a goddamn abomination.
// In spite of this, however, I'm somewhat proud of myself for
// having made it. So, if all you want is speed, here it is.

namespace lyo
{
    namespace ECS
    {
        // A fully stack-based component manager.
        // For template parameters, use a pack of ECS::comp::info structs.
        template <typename... Is>
            requires(sizeof...(Is) > 0 && sizeof...(Is) <= std::numeric_limits<comp::ID>::max())
        class static_component_manager
        {
        public:
            using type_list = lyo::type_list<typename Is::type...>;

            constexpr static_component_manager() = default;

            constexpr ~static_component_manager()
            {
                // Only clear if at least one type isn't trivially destructible.
                if constexpr (!(std::is_trivially_destructible_v<typename Is::type> || ...))
                    (this->clear<typename Is::type>(), ...);
            }

            template <in_infos<Is...> C, typename... Args>
            constexpr comp::index add(Args&&... args)
            {
                constexpr std::size_t begin { index_begin_v<C, Is...> },
                    end { begin + amount<C>() };

                const std::size_t idx { this->find_in_bitset<false>(begin, end) },
                    rel { idx - begin };

                assert(idx != end);

                new (type_begin<C>() + rel) C { std::forward<Args>(args)... };
                m_used.set(idx);

                return static_cast<comp::index>(rel);
            }

            template <in_infos<Is...> C>
            constexpr void remove(comp::index idx)
            {
                if (idx == comp::invalid_index)
                    return;

                assert(idx < amount<C>());
                assert((m_used[index_begin_v<C, Is...> + idx] == true));

                if (!std::is_trivially_destructible_v<C>)
                    this->get<C>(idx).~C();

                m_used.reset(index_begin_v<C, Is...> + idx);
            }

            template <in_infos<Is...> C>
            constexpr C& get(comp::index idx)
            {
                return type_begin<C>()[idx];
            }

            template <in_infos<Is...> C>
            constexpr const C& get(comp::index idx) const
            {
                return type_begin<C>()[idx];
            }

            template <in_infos<Is...> C>
            constexpr static comp::index amount()
            {
                return associated_type_t<C, Is...>::amount();
            }

            constexpr static comp::index components()
            {
                return sizeof...(Is);
            }

            template <in_infos<Is...> C>
            constexpr static comp::ID id()
            {
                return lyo::index_v<C, typename Is::type...>;
            }

        private:
            template <in_infos<Is...> C>
            constexpr void clear()
            {
                constexpr std::size_t bgn { index_begin_v<C, Is...> };
                constexpr std::size_t end { bgn + amount<C>() };

                for (comp::index i { 0 }; i < amount<C>(); ++i)
                {
                    type_begin<C>()[i].~C();
                    for (std::size_t i { bgn }; i != end; ++i)
                        m_used.reset(i);
                }
            }

            template <bool B>
            constexpr std::size_t find_in_bitset(std::size_t begin, std::size_t end)
            {
                for (; m_used[begin] != B && begin != end; ++begin)
                    ;
                return begin;
            }

            template <in_infos<Is...> C>
            constexpr C* type_begin()
            {
                return reinterpret_cast<C*>(m_arena.data() + byte_begin_v<C, Is...>);
            }

            template <in_infos<Is...> C>
            constexpr const C* type_begin() const
            {
                return reinterpret_cast<const C*>(m_arena.data() + byte_begin_v<C, Is...>);
            }

            template <in_infos<Is...> C>
            constexpr C* type_end()
            {
                return type_begin<C>() + amount<C>();
            }

            template <in_infos<Is...> C>
            constexpr const C* type_end() const
            {
                return type_begin<C>() + amount<C>();
            }

            std::array<std::byte, total_size_bytes_v<Is...>> m_arena;

            std::bitset<total_amount_v<Is...>> m_used;
        };
    }
}