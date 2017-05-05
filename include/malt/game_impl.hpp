//
// Created by Mehmet Fatih BAKIR on 27/04/2017.
//

#include <malt/game.hpp>
#include <iostream>

namespace malt
{
    template <class T>
    template <class MsgT, class... Args>
    void game<T>::deliver(entity_id id, MsgT, const Args& ... args)
    {
        mp::for_each(module_ts{}, [id, &args...](auto* module)
        {
            using module_t = std::remove_pointer_t<decltype(module)>;
            mp::for_each(typename module_t::component_ts{}, [id, &args...](auto* comp)
            {
                using comp_t = std::remove_pointer_t<decltype(comp)>;
                module_t:: template get_mgr<comp_t>().deliver(id, MsgT{}, args...);
            });
        });
    }

    template <class T>
    template<class MsgT, class... Args>
    void game<T>::broadcast(MsgT, const Args& ... args)
    {
        mp::for_each(module_ts{}, [&args...](auto* module)
        {
            using module_t = std::remove_pointer_t<decltype(module)>;
            mp::for_each(typename module_t::component_ts{}, [&args...](auto* comp)
            {
                using comp_t = std::remove_pointer_t<decltype(comp)>;
                module_t:: template get_mgr<comp_t>().broadcast(MsgT{}, args...);
            });
        });
    }

    template <class T>
    entity game<T>::create_entity()
    {
        return entity(next++);
    }

    entity create_entity()
    {
        return impl::create_entity();
    }

    template <class T>
    void game<T>::synchronize()
    {
        mp::for_each(module_ts{}, [this](auto* module)
        {
            using module_t = std::remove_pointer_t<decltype(module)>;
            mp::for_each(typename module_t::component_ts{}, [&](auto* comp)
            {
                using comp_t = std::remove_pointer_t<decltype(comp)>;
                this->get_mgr<comp_t>().synchronize();
            });
        });
    }
}

#define MALT_IMPLEMENT_GAME(GAME_T) \
    template class malt::game<GAME_T>;
