//
// Created by fatih on 4/27/17.
//

#pragma once

#include <malt/malt_fwd.hpp>
#include <malt/entity.hpp>
#include "message.hpp"

namespace malt
{
    class component
    {
        entity m_e;
        bool m_enabled = true;

        template <class T>
        friend class game;

        template <class T>
        friend class component_mgr;


    public:
        component() = default;
        component(const component&) = delete;
        component(component&&) = default;
        entity get_entity() const;

        component& operator=(const component&) = delete;
        component& operator=(component&&) = default;

        bool is_enabled() const { return m_enabled; }
        void set_enabled(bool active) { m_enabled = active; }

        template <class CompT>
        CompT* get_component() {
            return m_e.get_component<CompT>();
        }
    };
}

