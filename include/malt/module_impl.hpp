//
// Created by fatih on 4/27/17.
//

#include <malt/module.hpp>

namespace malt
{
}

#define MALT_IMPLEMENT_MODULE(MODULE_DEF_T) \
    template class malt::module<MODULE_DEF_T>;
