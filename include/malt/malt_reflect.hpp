//
// Created by fatih on 6/2/17.
//

#pragma once

#include <cstddef>
#include <malt/malt_fwd.hpp>
#include <malt/component_traits.hpp>

namespace malt
{
    namespace reflection
    {
        class icomponent;
        struct icomponent_range
        {
            const icomponent **m_begin, **m_end;
        public:
            const icomponent** begin() const { return m_begin; }
            const icomponent** end() const { return m_end; }
        };

        class icomponent
        {
        public:
            virtual const char* get_name() const = 0;
            virtual size_t get_type_hash() const = 0;
            virtual module_id get_module_id() const = 0;
            virtual malt::component* add_component(entity_id id) const = 0;
            virtual icomponent_range get_base_components() const = 0;
            virtual ~icomponent() = default;
        };
    }
}
