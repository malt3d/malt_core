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
        using component_mgr_ts = malt::mp::map_t<malt::mp::mapper<component_mgr>, component_ts>;
        using component_mgr_tuples = malt::mp::convert_t<std::tuple, component_mgr_ts>;

    private:
        static component_mgr_tuples comp_mgrs;

        friend class entity;

        template <class>
        friend class game;

        template<class CompT>
        static component_mgr<CompT>& get_mgr()
        {
            constexpr auto index = malt::mp::index_of_t<CompT, component_ts>();
            static_assert(index >= 0, "Component doesn't belong to module");
            return std::get<index>(comp_mgrs);
        }
    };
}

