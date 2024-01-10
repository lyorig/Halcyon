#pragma once

#include <algorithm>
#include <iostream>
#include <lyoSTL/concepts.hpp>

namespace lyo
{
    namespace ecs
    {
        template <template <typename...> typename Container, typename Comp_Mgr, template <typename> typename Entity>
        class scene
        {
        public:
            using entity  = Entity<Comp_Mgr>;
            using ent_vec = Container<entity>;

            template <typename... Comps>
            constexpr entity::ID spawn()
            {
                const typename entity::ID eid { m_lastID++ };

                entity e { eid };
                (e.template add<Comps>(m_holder), ...);
                m_ents.push_back(std::move(e));

                return eid;
            }

            constexpr void kill(ent_vec::iterator pos)
            {
                assert(pos != m_ents.end());

                // I don't know why, but if I try setting the type list as
                // a default argument, compilation fails. I love templates!
                this->free_entity(pos, typename Comp_Mgr::type_list {});

                m_ents.erase(pos);
            }

            template <typename... Comps>
            constexpr bool has(ent_vec::iterator pos) const
            {
                assert(pos != m_ents.end());
                return pos->template has<Comps...>(m_holder);
            }

            constexpr std::size_t ents() const
            {
                return m_ents.size();
            }

            constexpr std::size_t capacity() const
            {
                return m_ents.capacity();
            }

            constexpr entity& operator[](entity::ID eid)
            {
                auto iter = this->find(eid);
                assert(iter != m_ents.end()); // Entity must exist.

                return *iter;
            }

            constexpr const entity& operator[](entity::ID eid) const
            {
                const auto iter = this->find(eid);
                assert(iter != m_ents.end()); // Entity must exist.

                return *iter;
            }

            constexpr ent_vec::iterator find(entity::ID eid)
            {
                return std::find_if(m_ents.begin(), m_ents.end(), [&](const entity& ent)
                    { return ent.id() == eid; });
            }

            constexpr ent_vec::const_iterator find(entity::ID eid) const
            {
                return std::find_if(m_ents.begin(), m_ents.end(), [&](const entity& ent)
                    { return ent.id() == eid; });
            }

        protected:
            template <typename... Ts>
            constexpr void free_entity(ent_vec::iterator iter, lyo::type_list<Ts...>)
            {
                const typename entity::comp_vec& c { iter->comps() };

                (m_holder.template remove<Ts>(c[Comp_Mgr::template id<Ts>()]), ...);
            }

            Comp_Mgr m_holder {};

            ent_vec    m_ents {};
            entity::ID m_lastID { 0 };
        };
    }
}