//
// Created by fatih on 4/27/17.
//

#pragma once

#include <malt/list.hpp>
#include <malt/component_mgr.hpp>
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

