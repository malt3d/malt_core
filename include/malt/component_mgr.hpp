//
// Created by fatih on 4/27/17.
//

#pragma once

#include <malt/malt_fwd.hpp>
#include <memory>
#include <malt/component_traits.hpp>
#include <vector>
#include <malt/utilities.hpp>

namespace malt
{
    template <class CompT>
    class comp_mgr_priv;

    template <class CompT>
    class component_mgr
    {
        /*
         * we maintain 2 parallel component containers
         * newly added components are put into the aside container
         * this allows us to prevent any iterator invalidation during a frame
         */
        std::vector<CompT> comps;
        std::vector<CompT> aside; //TODO: use malt::buffer

        template <class>
        friend class game;

        template <class MsgT, class... Args>
        void deliver(std::true_type, entity_id id, MsgT, Args&&... args)
        {
            auto cmp = get_component(id);
            if (!cmp) return;
            cmp->Handle(MsgT{}, std::forward<Args>(args)...);
        };

        template <class MsgT, class... Args>
        void broadcast_impl(std::true_type, MsgT, Args&&... args)
        {
            for (auto& c : comps)
            {
                if (!detail::get_id(c.get_entity()))
                {
                    continue;
                }
                c.Handle(MsgT{}, std::forward<Args>(args)...);
            }
            for (auto& c : aside)
            {
                if (!detail::get_id(c.get_entity()))
                {
                    continue;
                }
                c.Handle(MsgT{}, std::forward<Args>(args)...);
            }
        };

        template <class MsgT, class... Args>
        void deliver(std::false_type, entity_id, MsgT, Args&&... args){};

        template <class MsgT, class... Args>
        void broadcast_impl(std::false_type, MsgT, Args&&... args){};

        void synchronize();

    public:
        component_mgr();
        ~component_mgr();
        CompT* get_component(entity_id id);
        CompT* add_component(entity_id id);

        void remove_component(CompT* c);

        template <class FunT>
        void for_each(const FunT& fun)
        {
            for (auto& c : comps)
            {
                if (!detail::get_id(c.get_entity()))
                {
                    continue;
                }
                fun(&c);
            }
            for (auto& c : aside)
            {
                if (!detail::get_id(c.get_entity()))
                {
                    continue;
                }
                fun(&c);
            }
        }

        template <class MsgT, class... Args>
        void deliver(entity_id id, MsgT, Args&&... args)
        {
            deliver(can_handle<MsgT(Args...)>::template value<CompT>(), id, MsgT{}, std::forward<Args>(args)...);
        };

        template <class MsgT, class... Args>
        void broadcast(MsgT, Args&&... args)
        {
            broadcast_impl(can_handle<MsgT(Args...)>::template value<CompT>(), MsgT{}, std::forward<Args>(args)...);
        };
    };
}

