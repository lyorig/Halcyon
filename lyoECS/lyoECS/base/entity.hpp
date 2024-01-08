#pragma once

#include <lyoECS/base/component.hpp>

namespace lyo
{
    namespace ECS
    {
        template <template <typename...> typename Container>
        class entity_base
        {
            using CID = comp::index;

        public:
            using ID       = lyo::u16;
            using comp_vec = Container<comp::index>;

            enum : ID
            {
                invalid_id = invalid<ID>
            };

            constexpr entity_base(ID eid)
                : m_id { eid }
            {
            }

            template <typename C, typename System>
            constexpr void add(System& sys)
            {
                m_comps.resize(std::size_t(sys.template id<C>()) + 1);
                m_comps[sys.template id<C>()] = sys.template add<C>();
            }

            template <typename C, typename System>
            constexpr void remove(System& sys)
            {
                sys.template remove<C>(m_comps[sys.template id<C>()]);

                m_comps[sys.template id<C>()] = comp::invalid_index;
            }

            template <typename C, typename System>
            constexpr C& get(System& sys)
            {
                HAL_DEBUG_ASSERT(this->has<C>(sys), "Getting non-existent component");

                return sys.template get<C>(this->index<C>());
            }

            template <typename C, typename System>
            comp::ID index(const System& sys)
            {
                assert(this->has<C>());
                return m_comps[sys.template cid<C>()];
            }

            template <typename C, typename System>
            constexpr const C& get(const System& sys) const
            {
                HAL_DEBUG_ASSERT(this->has<C>(sys), "Getting non-existent component");

                return sys.template get<C>(m_comps[sys.template id<C>()]);
            }

            template <typename System, typename... Cs>
            constexpr bool has(const System& sys)
            {
                return ((m_comps[sys.template id<Cs>()] != comp::invalid_index) && ...);
            }

            constexpr ID id() const
            {
                return m_id;
            }

            constexpr bool valid() const
            {
                return m_id != invalid_id;
            }

            const comp_vec& comps()
            {
                return m_comps;
            }

        protected:
            comp_vec m_comps;

        private:
            ID m_id;
        };
    }
}