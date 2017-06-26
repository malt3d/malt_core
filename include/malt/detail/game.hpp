//
// Created by Mehmet Fatih BAKIR on 27/04/2017.
//

#pragma once

#include <malt/utilities.hpp>
#include <malt/malt_fwd.hpp>
#include <malt/meta.hpp>
#include <malt/entity.hpp>
#include <unordered_map>
#include <tuple>
#include <malt/detail/entity_info.hpp>

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

    template <class CompT, bool include_self = true>
    struct is_base_of
    {
        template <class OtherT>
        static constexpr auto value() { return std::is_base_of<CompT, OtherT>{} || (include_self && std::is_same<CompT, OtherT>{}); }
    };

    template <class CompT, bool include_self = true>
    struct is_derived_from
    {
        template <class OtherT>
        static constexpr auto value() { return std::is_base_of<OtherT, CompT>{} && (!include_self && !std::is_same<CompT, OtherT>{}); }
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
        using mgr_ts = meta::map_t<meta::mapper<malt::component_mgr>, comp_ts>;
        using mgr_tuple_t = meta::convert_t<std::tuple, mgr_ts>;

        static constexpr auto component_count = meta::length_t(comp_ts{});

        std::unordered_map<int, comp_t_id> m_hash_to_index;
        std::vector<std::function<malt::component*(entity_id)>> m_erased_adders;
        std::vector<std::function<malt::component*(entity_id)>> m_erased_getters;
        std::vector<std::function<void(entity_id)>> m_erased_destroyers;

        mgr_tuple_t m_comp_managers;

        /*!
         * This field stores the id of the entity that will be created next
         */
        entity_id m_next_id = 1;
        detail::entity_manager<component_count> m_entity_manager;
    public:

        game();
        void diagnostics();

        void set_name(entity_id id, std::string name)
        {
            m_entity_manager.set_name(id, std::move(name));
        }

        const std::string& get_name(entity_id id)
        {
            return m_entity_manager.get_name(id);
        }

        malt::component* erased_add_component(comp_t_id c, entity_id e)
        {
            at_exit([this, e, c] {
                m_entity_manager.add_component(e, c);
            });
            return m_erased_adders[c](e);
        }

        malt::component* hash_add_component(size_t c_hash, entity_id e)
        {
            comp_t_id c = m_hash_to_index[c_hash];
            return erased_add_component(c, e);
        }

        malt::component* erased_get_component(comp_t_id c, entity_id e)
        {
            return m_erased_getters[c](e);
        }

        void erased_destory_component(comp_t_id c, entity_id e)
        {
            m_erased_destroyers[c](e);
        }

        malt::component* hash_get_component(size_t c_hash, entity_id e)
        {
            auto c = m_hash_to_index[c_hash];
            return erased_get_component(c, e);
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
            entity e = c->get_entity();
            m_entity_manager.remove_component(e.id, meta::index_of_t<CompT, comp_ts>());
            get_mgr(meta::type<CompT>{}).remove_component(c);
        }

        template <class CompT>
        void notify_add_comp(entity_id ent)
        {
            m_entity_manager.add_component(ent, meta::index_of_t<CompT, comp_ts>());
        }

        void destroy_entity(entity e)
        {
            at_exit([this, e]{
                m_entity_manager.remove_entity(e.id);
            });

            /*
             * loop over all components and destroy them
             */
        }

        auto get_component_map(entity id)
        {
            return m_entity_manager.get_components(id.id);
        }

        static constexpr auto get_component_type_count()
        {
            return component_count;
        }

        template <class CompT>
        component_mgr<CompT>& get_mgr(meta::type<CompT>)
        {
            constexpr auto index = meta::index_of_t<CompT, comp_ts>();
            return std::get<index>(m_comp_managers);
        }

        template <class T>
        erased_range<T, component> get_components(meta::type<T>);

        erased_range<entity_id, entity_id> get_entities();
    };

}
