#pragma once

#include <lyoECS/base/atp.hpp>
#include <lyoECS/base/scene.hpp>
#include <vector>

namespace lyo
{
    namespace ecs
    {
        template <typename Comp_Mgr, template <typename> typename Entity, std::size_t Max_Ents>
        using static_scene = scene<array_template_proxy<Max_Ents>::template type, Comp_Mgr, Entity>;

        template <typename Comp_Mgr, template <typename> typename Entity>
        using dynamic_scene = scene<std::vector, Comp_Mgr, Entity>;
    }
}