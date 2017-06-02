//
// Created by fatih on 4/29/17.
//

#pragma once

#include <functional>
#include <malt/malt_fwd.hpp>
#include <utility>
#include <malt/engine_defs.hpp>
#include <malt/malt_reflect.hpp>

namespace malt
{
    namespace impl
    {
        template <class CompT>
        struct component_adapter
        {
            static component_mgr<CompT>& get_mgr() MALT_WEAK_SYMBOL;
            static void destroy(CompT*) MALT_WEAK_SYMBOL;
            static erased_range<CompT, component> get_components() MALT_WEAK_SYMBOL;
        };

        template <class MsgT, class... Args> struct msg_delivery;

        template <class MsgT, class... Args>
        struct msg_delivery<MsgT(Args...)>
        {
            static void deliver(malt::entity_id id, MsgT, const Args&... args) MALT_WEAK_SYMBOL;
            static void broadcast(MsgT, const Args&... args) MALT_WEAK_SYMBOL;
        };

        entity create_entity() MALT_WEAK_SYMBOL;

        void post_frame() MALT_WEAK_SYMBOL;
        void terminate() MALT_WEAK_SYMBOL;
        bool is_terminated() MALT_WEAK_SYMBOL;
        float get_delta_time() MALT_WEAK_SYMBOL;

        malt::component* add_component(const comp_t_id& comp_type, entity_id) MALT_WEAK_SYMBOL;
    }

    /*
     * Broadcasts a given message to all entities in the game.
     * Message type must not contain any data, as the object itself is discarded
     * Data to be delivered to recipients should be passed using additional arguments 
     */
    template <class MsgT, class... Args>
    void broadcast(MsgT, const Args&... args) 
    {
        impl::msg_delivery<MsgT(Args...)>::broadcast(MsgT{}, args...);
    }

    /*
     * Returns an iterator pair that will iterate over components of type CompT
     * and components of type that derive from CompT
     */
    template <class CompT>
    erased_range<CompT, component> get_components()
    {
        return impl::component_adapter<CompT>::get_components();
    }

    /*
     * Creates a new entity
     */
    entity create_entity();

    /*
     * Destroys the given component
     */
    template <class CompT>
    void destroy(CompT comp)
    {
        using elem_t = typename std::iterator_traits<CompT>::value_type;
        impl::component_adapter<elem_t>::destroy(&(*comp));
    }

    /*
     * Peacefully shuts down the game
     */
    inline void terminate() { impl::terminate(); }

    inline bool is_terminated() { return impl::is_terminated(); }

    /*
     * Adds a component using the hash of the component type
     * This is useful for loading scenes from files
     */
    malt::component* add_component(const comp_t_id& c_id, entity_id e_id) MALT_PUBLIC;

    /*
     * Adds a component using the name of the component type
     * This is useful for loading scenes from files
     */
    malt::component* add_component(const char* comp_type_name, entity_id e_id) MALT_PUBLIC;

    /*
     * Gets runtime reflection information from a component
     */
    const reflection::icomponent_type* reflect(component* c) MALT_PUBLIC;

    namespace time
    {
        inline float get_delta_time() {
            return impl::get_delta_time();
        }
    }
}

