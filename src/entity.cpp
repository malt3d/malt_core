//
// Created by fatih on 4/27/17.
//

#include <malt/entity.hpp>

namespace malt
{
    namespace detail
    {
        entity_id get_id(const entity& e)
        {
            return e.id;
        }
    }
}
