//
// Created by fatih on 4/29/17.
//

#pragma once

#include <functional>
#include <malt/malt_fwd.hpp>
#include <utility>

namespace malt
{
    namespace impl
    {
        template <class CompT>
        struct component_adapter
        {
            static component_mgr<CompT>& get_mgr();
            static void for_components(std::function<void(CompT*)>);
            static void destroy(CompT*);
        };

        template <class MsgT, class... Args> struct msg_delivery;

        template <class MsgT, class... Args>
        struct msg_delivery<MsgT(Args...)>
        {
            static void deliver(malt::entity_id id, MsgT, const Args&... args);
            static void broadcast(MsgT, const Args&... args);
        };

        entity create_entity();

        void post_frame();
        void terminate();
        bool is_terminated();
    }

    template <class MsgT, class... Args>
    void broadcast(MsgT, const Args&... args) {
        impl::msg_delivery<MsgT(Args...)>::broadcast(MsgT{}, args...);
    };

    template <class CompT, class FunT>
    void for_each_component(const FunT& t)
    {
        impl::component_adapter<CompT>::for_components(t);
    };

    entity create_entity();

    template <class CompT>
    void destroy(CompT comp)
    {
        using elem_t = typename std::iterator_traits<CompT>::value_type;
        impl::component_adapter<elem_t>::destroy(&(*comp));
    }

    inline void terminate() { impl::terminate(); }
    inline bool is_terminated() { return impl::is_terminated(); }
}

