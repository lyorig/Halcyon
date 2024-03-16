#pragma once

#include <quest/ents/types.hpp>
#include <tuple>

namespace quest
{
    template <typename T>
    concept semgr_compatible = std::is_base_of_v<ent::mod::unique, T> && lyo::non_cv<T>;

    template <semgr_compatible... Ents>
    class singleton_entity_manager_tmpl
    {
    public:
        template <typename F>
        void visit(F&& func)
        {
            ([&]()
                {
                    if constexpr (std::is_invocable_v<F, Ents>)
                        func(std::get<Ents>(tuple)); }(),
                ...);
        }

        std::tuple<Ents...> tuple;
    };

    using singleton_entity_manager = singleton_entity_manager_tmpl<ent::player>;
}