//
// Created by fatih on 10/30/17.
//

#include <malt/engine.hpp>
#include <malt/entity.hpp>

namespace malt
{
    entity find_entity(const std::string& name)
    {
        return impl::find_entity(name);
    }
}