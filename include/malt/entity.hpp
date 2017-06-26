//
// Created by fatih on 4/27/17.
//

#pragma once

#include <malt/malt_fwd.hpp>
#include <malt/utilities.hpp>

namespace malt {
    class entity
    {
        entity_id id;

        template<class T>
        friend
        class game;

        friend entity_id detail::get_id(const entity& e);

    public:
        entity()
                :id(0)
        { }

        explicit entity(entity_id id)
                :id(id)
        { }

        template<class T>
        T* get_component();

        template<class T>
        T* add_component();

        template<class MsgT, class... ArgTs>
        void deliver_message(MsgT, const ArgTs& ... args);

        void set_name(std::string name);
        const std::string& get_name() const;
    };
}

#include <malt/engine.hpp>

namespace malt{
    template<class T>
    T* entity::get_component()
    {
        return impl::component_adapter<T>::get_component(id);
    }

    template<class T>
    T* entity::add_component()
    {
        return impl::component_adapter<T>::add_component(id);
    }

    template<class MsgT, class... ArgTs>
    void entity::deliver_message(MsgT, const ArgTs& ... args)
    {
        impl::msg_delivery<MsgT(ArgTs...)>::deliver(id, MsgT{}, args...);
    }

    inline
    void entity::set_name(std::string name)
    {
        impl::set_name(id, std::move(name));
    }

    inline
    const std::string&
    entity::get_name() const
    {
        return impl::get_name(id);
    }
}
