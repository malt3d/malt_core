//
// Created by Mehmet Fatih BAKIR on 27/04/2017.
//

#pragma once

#include <tuple>
#include "malt_fwd.hpp"
#include "list.hpp"
#include <malt/entity.hpp>
#include <unordered_map>

namespace malt
{
    template <class CompT, class ModuleT>
    constexpr bool has_component_impl()
    {
        return meta::index_of_t<CompT, typename ModuleT::component_ts>() >= 0;
    }

    template <class CompT>
    struct has_component
    {
        template <class ModuleT>
        static constexpr auto value(){ return has_component_impl<CompT, ModuleT>(); }
    };

    template <class CompT>
    struct is_base_of
    {
        template <class OtherT>
        static constexpr auto value() { return std::is_base_of<CompT, OtherT>{} || std::is_same<CompT, OtherT>{}; }
    };

    struct get_comps
    {
        template <class ModuleT>
        using invoke = typename ModuleT::component_ts;
    };


    struct get_range_t
    {
        template <class>
        using invoke = erased_containers<component>;
    };

    template <class> struct print;

    template <class GameConfig>
    class game
    {
        using module_ts = typename GameConfig::modules;
        using comp_ts = meta::merge_t<meta::map_t<get_comps, module_ts>>;

        std::unordered_map<comp_t_id, std::function<malt::component*(entity_id)>> erased_adders;

        entity_id next = 1;
    public:

        game();
        void diagnostics();

        malt::component* erased_add_component(comp_t_id c, entity_id e)
        {
            return erased_adders[c](e);
        }

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
            get_mgr(meta::type<CompT>{}).remove_component(c);
        }

        template <class CompT>
        component_mgr<CompT>& get_mgr(meta::type<CompT>)
        {
            using mapped = meta::filter_t<has_component<CompT>, module_ts>;
            return meta::front_t<mapped>:: template get_mgr<CompT>();
        }

        template <class T>
        erased_range<T, component> get_components(meta::type<T>);
    };

}
