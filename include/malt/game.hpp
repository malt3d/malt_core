//
// Created by Mehmet Fatih BAKIR on 27/04/2017.
//

#pragma once

#include "malt_fwd.hpp"
#include "list.hpp"
#include <malt/entity.hpp>

namespace malt
{
    template <class CompT, class ModuleT>
    constexpr bool has_component_impl()
    {
        return mp::index_of_t<CompT, typename ModuleT::component_ts>() >= 0;
    }

    template <class CompT>
    struct has_component
    {
        template <class ModuleT>
        static constexpr auto value(){ return has_component_impl<CompT, ModuleT>(); }
    };

    template <class GameConfig>
    class game
    {
        using module_ts = typename GameConfig::modules;

        entity_id next = 1;
    public:
        template <class MsgT, class... Args>
        void deliver(malt::entity_id id, MsgT, const Args&...);

        template <class MsgT, class... Args>
        void broadcast(MsgT, const Args&...);

        entity create_entity();

        void synchronize();

        template <class CompT>
        void destroy_comp(CompT* c)
        {
            if (!c) return;
            get_mgr<CompT>().remove_component(c);
        }

        template <class CompT>
        component_mgr<CompT>& get_mgr()
        {
            using mapped = mp::filter_t<has_component<CompT>, module_ts>;
            return mp::front_t<mapped>:: template get_mgr<CompT>();
        }
    };

}
