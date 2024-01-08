#pragma once

#include <lyoECS/base/atp.hpp>
#include <lyoECS/base/entity.hpp>

namespace lyo
{
    namespace ECS
    {
        template <typename Comp_Mgr>
        using static_entity = entity_base<array_template_proxy<Comp_Mgr::components()>::template type>;
    }
}