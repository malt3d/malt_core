//
// Created by fatih on 4/27/17.
//

#pragma once

#include <malt/malt_fwd.hpp>
#include <malt/utilities.hpp>

namespace malt {

    /*
     * Entity definition of malt's entity component architecture
     * An entity is identified by an integer and entity objects
     * do not store any data regarding an actual _entity_.
     *
     * This class exists so that some common functionality about
     * entities can be used as a regular object:
     *      auto some_comp = entity.get_component<transform>();
     *      // instead of
     *      auto some_comp = malt::get_component<transform>(entity);
     */
    class entity
    {
        /*
         * identifier of this entity
         */
        entity_id id;

        template<class T>
        friend class game;
        friend entity_id detail::get_id(const entity& e);

    public:
        constexpr entity()
                :id(0)
        { }

        explicit constexpr entity(entity_id id)
                :id(id)
        { }

        template<class T>
        T* get_component();

        template<class T>
        T* add_component();

        /*
         * delivers the given message to all components of this entity
         */
        template<class MsgT, class... ArgTs>
        void deliver_message(MsgT, const ArgTs& ... args);

        /*
         * sets the name of this entity
         *
         * names live in a global vector, therefore updating the name
         * through one entity object updates the name for all entitites
         * sharing the same id
         */
        void set_name(std::string name);

        /*
         * gets the name of this entity
         */
        const std::string& get_name() const;

        bool operator==(const entity& rhs)
        {
            return id == rhs.id;
        }
    };

    static constexpr entity nullentity(0);
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
