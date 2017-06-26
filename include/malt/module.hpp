//
// Created by fatih on 4/27/17.
//

#pragma once

#include <malt/meta.hpp>
#include <malt/component.hpp>
#include <tuple>

namespace malt
{
    template <class T>
    class module
    {
    public:
        using component_ts = typename T::components;

    private:

        friend class entity;

        template <class>
        friend class game;
    };
}

#define MALT_IMPLEMENT_MODULE(MODULE_DEF_T) \
    template class malt::module<MODULE_DEF_T>;