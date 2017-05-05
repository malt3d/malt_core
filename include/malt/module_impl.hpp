//
// Created by fatih on 4/27/17.
//

#include <malt/module.hpp>

namespace malt
{
    template <class T> typename module<T>::component_mgr_tuples module<T>::comp_mgrs;
}

#define MALT_IMPLEMENT_MODULE(MODULE_DEF_T) \
    template class malt::module<MODULE_DEF_T>;
