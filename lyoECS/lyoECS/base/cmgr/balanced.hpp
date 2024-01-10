#pragma once

#include <algorithm>
#include <cassert>
#include <lyoECS/base/component.hpp>
#include <vector>

namespace lyo
{
    namespace ecs
    {
        // A fully stack-based component manager.
        // For template parameters, use a pack of comp::info structs.
        template <typename... Is>
            requires(sizeof...(Is) > 0 && sizeof...(Is) <= std::numeric_limits<comp::ID>::max())
        class balanced_component_manager
        {
            using byte_vec = std::vector<std::byte>;
            using bool_vec = std::vector<bool>;

        public:
            using type_list = lyo::type_list<typename Is::type...>;

            constexpr balanced_component_manager() = default;

            constexpr ~balanced_component_manager()
            {
                (this->clear<typename Is::type>(), ...);
            }

            template <in_infos<Is...> C, typename... Args>
            constexpr comp::index add(Args&&... args)
            {
                byte_vec& vec { m_comps[id<C>()] };
                bool_vec& bvec { m_used[id<C>()] };

                auto iter = std::find(bvec.begin(), bvec.end(), false);

                const std::size_t idx { std::size_t(std::distance(bvec.begin(), iter)) };

                if (iter == bvec.end()) // Need to make room.
                {
                    vec.reserve(vec.size() + sizeof(C));
                    bvec.push_back(true);
                }

                else
                    bvec[idx] = true;

                new (type_begin<C>() + idx) C { std::forward<Args>(args)... };

                return idx;
            }

            template <in_infos<Is...> C>
            constexpr void remove(comp::index idx)
            {
                if (idx == comp::invalid_index)
                    return;

                assert(idx < amount<C>());

                this->get<C>(idx).~C();

                m_used[id<C>()][idx] = false;
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
            constexpr comp::index amount()
            {
                return m_comps[id<C>()].size();
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
                for (byte_vec vec : m_comps)
                    for (comp::index i { 0 }; i < vec.size(); ++i)
                        this->get<C>(i).~C();
            }

            template <in_infos<Is...> C>
            constexpr C* type_begin()
            {
                return reinterpret_cast<C*>(m_comps[id<C>()].data());
            }

            template <in_infos<Is...> C>
            constexpr const C* type_begin() const
            {
                return reinterpret_cast<const C*>(m_comps[id<C>()].data());
            }

            std::array<byte_vec, components()> m_comps;
            std::array<bool_vec, components()> m_used;
        };
    }
}