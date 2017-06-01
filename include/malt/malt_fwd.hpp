//
// Created by fatih on 4/27/17.
//

#include <cstdint>

namespace malt
{
    using entity_id = uint32_t;
    using comp_t_id = uint32_t;
    using module_id = uint32_t;

    class entity;
    class component;

    template <class> class component_mgr;
    template <class> class module;

    template <class> class game;

    template <class, class>
    struct erased_range;
}
