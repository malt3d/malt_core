//
// Created by fatih on 4/27/17.
//

#include <malt/component.hpp>
#include <malt/engine.hpp>
#include <malt/utilities.hpp>

namespace malt
{
    entity component::get_entity() const
    { return m_e; }

    component* add_component(const char* comp_type_name, entity_id e_id)
    {
        return impl::add_component(hash_c_string(comp_type_name, strlen(comp_type_name)), e_id);
    }

    malt::component* add_component(const comp_t_id& c_id, entity_id e_id)
    {
        return impl::add_component(c_id, e_id);
    }

    const reflection::icomponent* reflect(component* c)
    {
        return c->m_type;
    }
}
