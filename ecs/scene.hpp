#pragma once

#include <vector>

#include "component.hpp"
#include "entity.hpp"
#include "pool.hpp"

/* scene.hpp:
   An Entity-Component System scene.
   Inspired by David Colson's guide.
   https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html */

namespace ecs
{
    struct end_iter_t
    {
    } constexpr end_iter;

    class scene
    {
      public:

        template <typename... Comps>
        class view  // Iterable view of entities with desired components.
        {
          public:

            class iterator
            {
              public:

                constexpr iterator(scene& scn) noexcept :
                    m_ents { scn.m_ents },
                    m_idx { 0 }
                {
                    if constexpr (sizeof...(Comps) > 0)
                    {
                        for (auto cid : { scn.CID<Comps>()... })
                            m_mask.set(cid);

                        while (m_idx != m_ents.size() && (!m_ents[m_idx].id.valid() || m_mask != (m_mask & m_ents[m_idx].mask)))
                            ++m_idx;  // Find the first valid index.
                    }
                }

                constexpr iterator(scene& scn, end_iter_t) noexcept :
                    m_ents { scn.m_ents },
                    m_idx { static_cast<entity::index>(m_ents.size()) }
                {
                }

                entity::ID& operator*() const noexcept
                {
                    return m_ents[m_idx].id;
                }

                entity::ID* operator->() noexcept
                {
                    return &m_ents[m_idx].id;
                }

                iterator& operator++() noexcept
                {
                    if constexpr (sizeof...(Comps) == 0)  // Wildcard view.
                    {
                        do
                        {
                            ++m_idx;
                        } while (m_idx != m_ents.size() && !m_ents[m_idx].id.valid());
                    }

                    else  // Specific view.
                    {
                        do
                        {
                            ++m_idx;
                        } while (m_idx != m_ents.size() && (!m_ents[m_idx].id.valid() || m_mask != (m_mask & m_ents[m_idx].mask)));
                    }

                    return *this;
                }

                iterator operator++(int)
                {
                    iterator temp { *this };

                    this->operator++();

                    return temp;
                }

                bool operator==(const iterator& other) const noexcept
                {
                    return m_idx == other.m_idx;
                }

                bool operator!=(const iterator& other) const noexcept
                {
                    return m_idx != other.m_idx;
                }

              private:

                std::vector<entity>& m_ents;
                entity::index        m_idx;

                MAYBE_EMPTY std::conditional_t<sizeof...(Comps) == 0, end_iter_t, component::mask> m_mask;
            };

            view(scene& scn) noexcept :
                m_scene { scn }
            {
            }

            iterator begin() noexcept
            {
                return { m_scene };
            }

            iterator end() noexcept
            {
                return { m_scene, end_iter };
            }

          private:

            scene& m_scene;
        };

        scene() noexcept :
            m_currentCID { 0 }
        {
        }

        entity::ID new_entity() noexcept;

        template <typename Comp>
        component::index CID() noexcept
        {
            static const component::index cid { m_currentCID++ };

            return cid;
        }

        template <typename Comp, typename... Args>
        Comp* assign(entity::ID id, Args... args) noexcept
        {
            if (m_ents[id.idx].id != id)  // Deleted entity check.
                return nullptr;

            const component::index cid { CID<Comp>() };

            if (m_pools.size() <= cid)  // New component.
                m_pools.emplace_back(sizeof(Comp));

            m_ents[id.idx].mask.set(CID<Comp>());

            return new (m_pools[cid][id.idx]) Comp { args... };
        }

        template <typename Comp>
        void remove(entity::ID id) noexcept
        {
            if (m_ents[id.idx].id != id)  // Deleted entity check.
                return;

            m_free.push_back(id.idx);  // Mark as free.

            m_ents[id.idx].mask.reset(CID<Comp>());
        }

        template <typename Comp>
        Comp* get(entity::ID id) noexcept
        {
            if (m_ents[id.idx].id != id)  // Deleted entity check.
                return nullptr;

            const component::index cid { CID<Comp>() };

            if (!m_ents[id.idx].mask[cid])
                return nullptr;

            return reinterpret_cast<Comp*>(m_pools[cid][id.idx]);
        }

        void destroy(entity::ID id) noexcept
        {
            entity& ent { m_ents[id.idx] };

            ent.id.idx = entity::invalid_index();
            ent.id.ver += 1;

            ent.mask.reset();

            m_free.emplace_back(id.idx);
        }

        constexpr const std::vector<entity>& ents() const noexcept
        {
            return m_ents;
        }

        constexpr const std::vector<entity::index>& free() const noexcept
        {
            return m_free;
        }

        constexpr const std::vector<pool>& pools() const noexcept
        {
            return m_pools;
        }

      private:

        std::vector<entity>        m_ents;
        std::vector<entity::index> m_free;
        std::vector<pool>          m_pools;

        // Component IDs are usually implemented as a static variable and function.
        // I've decided to make them bound to a class instance, so as to comply with
        // Halcyon's "philosophy" of not allocating memory the user didn't ask for.
        component::index m_currentCID;
    };
}  // namespace ecs