//g
// Created by fatih on 4/27/17.
//

#include <malt/component.hpp>
#include <malt/component_mgr.hpp>
#include <vector>
#include <algorithm>
#include <malt/entity.hpp>
#include <iostream>
#include <malt/message.hpp>
#include <malt/utilities.hpp>

namespace malt
{
    template <class CompT>
    component_mgr<CompT>::component_mgr()
    {
        aside.reserve(64);
    }

    template <class CompT>
    CompT* component_mgr<CompT>::add_component(entity_id id)
    {
        aside.emplace_back();
        aside.back().m_e = entity(id);

        auto res = &aside.back();
        try_dispatch(res, init{});
        return res;
    }

    template <class CompT>
    CompT* component_mgr<CompT>::get_component(entity_id id)
    {
        auto res = std::find_if(comps.begin(), comps.end(), [&id](const CompT& c)
        {
            return detail::get_id(c.get_entity()) == id;
        });

        if(res != comps.end()) return &(*res);

        auto res_ = std::find_if(aside.begin(), aside.end(), [&id](const CompT& c)
        {
            return detail::get_id(c.get_entity()) == id;
        });

        if(res_ != aside.end()) return &(*res_);

        return nullptr;
    }

    template <class T>
    void component_mgr<T>::synchronize()
    {
        auto comp_it = comps.begin();
        auto aside_it = aside.begin();

        while (comp_it != comps.end() && aside_it != aside.end())
        {
            if (detail::get_id(comp_it->get_entity()))
            {
            }
            else
            {
                if (detail::get_id(aside_it->get_entity()))
                {
                    try_dispatch(aside_it, start{});
                    *comp_it = std::move(*aside_it);
                }
                ++aside_it;
            }
            ++comp_it;
        }

        for(auto bcomp_it = std::back_inserter(comps); aside_it != aside.end(); ++aside_it, ++bcomp_it)
        {
            if (detail::get_id(aside_it->get_entity()))
            {
                try_dispatch(aside_it, start{});
                *bcomp_it = std::move(*aside_it);
            }
        }

        aside.clear();
    }

    template <class CompT>
    void component_mgr<CompT>::remove_component(CompT* c)
    {
        try_dispatch(c, destruct{});
        c->m_e = entity(0);
    }

    template <class CompT>
    void component_mgr<CompT>::place_containers(erased_containers<component>& conts)
    {
        conts.m_begin_container = get_container<component>(comps);
        conts.m_end_container = get_container<component>(aside);
        conts.m_begin_container.m_next = &conts.m_end_container;
    }

    template <class CompT>
    component_mgr<CompT>::~component_mgr() = default;
}

#define MALT_IMPLEMENT_COMP(COMPT) \
    template class malt::component_mgr<COMPT>;
