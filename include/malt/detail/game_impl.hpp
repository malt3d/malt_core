//
// Created by Mehmet Fatih BAKIR on 27/04/2017.
//

#include <malt/detail/game.hpp>
#include <iostream>
#include <boost/type_index.hpp>
#include <malt/detail/component_mgr_impl.hpp>

namespace malt
{
    template <class T>
    template <class MsgT, class... Args>
    void game<T>::deliver(entity_id id, MsgT, const Args& ... args)
    {
        meta::for_each(comp_ts{}, [this, id, &args...](auto* c){
            using comp_t = std::remove_pointer_t<decltype(c)>;
            this->get_mgr(meta::type<comp_t>{}).deliver(id, MsgT{}, args...);
        });
    }

    template <class T>
    template<class MsgT, class... Args>
    void game<T>::broadcast(MsgT, const Args& ... args)
    {
        meta::for_each(comp_ts{}, [this, &args...](auto* c){
            using comp_t = std::remove_pointer_t<decltype(c)>;
            this->get_mgr(meta::type<comp_t>{}).broadcast(MsgT{}, args...);
        });
    }

    template <class T>
    entity game<T>::create_entity()
    {
        auto id = m_next_id++;
        m_entity_manager.add_entity(id, "Unnamed entity");
        return entity(id);
    }

    entity create_entity()
    {
        return impl::create_entity();
    }

    entity create_entity(std::string name)
    {
        auto&& e = create_entity();
        e.set_name(name);
        return e;
    }

    void destroy(entity e)
    {
        impl::destroy(e);
    }


    template <class T>
    void game<T>::synchronize()
    {   meta::for_each(comp_ts{}, [this](auto* c){
            using comp_t = std::remove_pointer_t<decltype(c)>;
            this->get_mgr(meta::type<comp_t>{}).synchronize();
        });
    }

    /*
     * this stupid struct is used because for some reason the linker complains if we directly
     * instantiate the tuple type in the static template function
     */
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

        // see the comment on the stupid_workaround class
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
    erased_range<entity_id, entity_id>
    game<GameT>::get_entities()
    {
        return m_entity_manager.get_entities();
    };

    template <class GameT>
    void game<GameT>::diagnostics()
    {
        meta::for_each(comp_ts{}, [](auto* c){
            using type1 = std::remove_pointer_t<decltype(c)>;
            std::cout << " + " << boost::typeindex::type_id<type1>().pretty_name() << '\n';
        });
    }

    template <class... ElemTs, size_t... Indices>
    auto init_mgrs(meta::list<ElemTs...>, std::index_sequence<Indices...>)
    {
        return std::make_tuple(ElemTs(Indices)...);
    };

    template <class GameT>
    game<GameT>::game() :
        m_comp_managers(init_mgrs(mgr_ts{}, std::make_index_sequence<component_count>()))
    {
        m_erased_getters.reserve(component_count);
        m_erased_adders.reserve(component_count);
        meta::for_each(comp_ts{}, [this](auto*c)
        {
            using type1 = std::remove_pointer_t<decltype(c)>;
            static constexpr auto index = meta::index_of_t<type1, comp_ts>();
            auto name = static_reflect(meta::type<type1>{}).name;

            m_hash_to_index[hash_c_string(name, strlen(name))] = index;

            m_erased_adders.emplace_back([&](entity_id id) -> malt::component*
            {
                return this->get_mgr(meta::type<type1>()).add_component(id);
            });

            m_erased_getters.emplace_back([&](entity_id id) -> malt::component*
            {
                return this->get_mgr(meta::type<type1>()).get_component(id);
            });

            m_erased_destroyers.emplace_back([&](entity_id id) -> void
            {
                auto comp = get_mgr(meta::type<type1>{}).get_component(id);
                this->destroy_comp(comp);
            });

            using derived_from_T = meta::filter_t<is_derived_from<type1, false>, comp_ts>;
            auto type = static_cast<const reflection::component_type<type1>*>(this->get_mgr(meta::type<type1>()).get_type());

            meta::for_each(derived_from_T{}, [this, type](auto* c)
            {
                using base_t = std::remove_pointer_t<decltype(c)>;
                type->add_base(this->get_mgr(meta::type<base_t>()).get_type());
            });
        });
    }
}

#define MALT_IMPLEMENT_GAME(GAME_T) \
    template class malt::game<GAME_T>;
