//
// Created by Mehmet Fatih BAKIR on 27/04/2017.
//

#include <malt/game.hpp>
#include <iostream>
#include <boost/type_index.hpp>

namespace malt
{
    template <class T>
    template <class MsgT, class... Args>
    void game<T>::deliver(entity_id id, MsgT, const Args& ... args)
    {
        meta::for_each(module_ts{}, [id, &args...](auto* module)
        {
            using module_t = std::remove_pointer_t<decltype(module)>;
            meta::for_each(typename module_t::component_ts{}, [id, &args...](auto* comp)
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
        meta::for_each(module_ts{}, [&args...](auto* module)
        {
            using module_t = std::remove_pointer_t<decltype(module)>;
            meta::for_each(typename module_t::component_ts{}, [&args...](auto* comp)
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
        meta::for_each(module_ts{}, [this](auto* module)
        {
            using module_t = std::remove_pointer_t<decltype(module)>;
            meta::for_each(typename module_t::component_ts{}, [&](auto* comp)
            {
                using comp_t = std::remove_pointer_t<decltype(comp)>;
                this->get_mgr(meta::type<comp_t>{}).synchronize();
            });
        });
    }

    template <class T, class U>
    struct stupid_workaround
    {
        U u;
    };

    template <class GameT>
    template<class T>
    erased_range<T, component> game<GameT>::get_components(meta::type<T>)
    {
        using derived_from_T = meta::filter_t<is_base_of<T>, comp_ts>;
        using erased_types = meta::map_t<get_range_t, derived_from_T>;
        using tuple_t = meta::convert_t<std::tuple, erased_types>;

        //std::cout << boost::typeindex::type_id<derived_from_T>().pretty_name() << '\n';
        //std::cout << boost::typeindex::type_id<tuple_t>().pretty_name() << '\n';
        thread_local stupid_workaround<T, tuple_t> erased_storage_;

        auto& erased_storage = erased_storage_.u;

        /*
         * TODO: this should run only once, make it static using the immediately evaluated lambda expressions
         */
        meta::for_each2(derived_from_T{}, [&](auto* c, auto* c2)
        {
            using type1 = std::remove_pointer_t<decltype(c)>;
            using type2 = std::remove_pointer_t<decltype(c2)>;
            constexpr auto index1 = meta::index_of_t<type1, derived_from_T>();
            constexpr auto index2 = meta::index_of_t<type2, derived_from_T>();
            std::get<index1>(erased_storage).m_end_container.m_next = &std::get<index2>(erased_storage).m_begin_container;
        });

        meta::for_each(derived_from_T{}, [&, this](auto* c){
            using type1 = std::remove_pointer_t<decltype(c)>;
            constexpr auto index1 = meta::index_of_t<type1, derived_from_T>();
            //std::cout << boost::typeindex::type_id<type1>().pretty_name() << '\n';
            this->get_mgr(meta::type<type1>{}).place_containers(std::get<index1>(erased_storage));
        });

        return erased_range<T, component>{std::get<0>(erased_storage).m_begin_container};
    }

    template <class GameT>
    void game<GameT>::diagnostics()
    {
        meta::for_each(comp_ts{}, [](auto* c){
            using type1 = std::remove_pointer_t<decltype(c)>;
            std::cout << " + " << boost::typeindex::type_id<type1>().pretty_name() << '\n';
        });
    }
}

#define MALT_IMPLEMENT_GAME(GAME_T) \
    template class malt::game<GAME_T>;
