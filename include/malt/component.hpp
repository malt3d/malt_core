//
// Created by fatih on 4/27/17.
//

#pragma once

#include <malt/engine_defs.hpp>
#include <malt/malt_fwd.hpp>
#include <malt/entity.hpp>
#include <malt/malt_reflect.hpp>
#include <malt/track_ptr.hpp>
#include <malt/message.hpp>

namespace malt
{
    class MALT_PUBLIC component : public tracked
    {
        entity m_e;
        const reflection::icomponent* m_type;
        bool m_enabled = true;

        template <class T>
        friend class game;

        template <class T>
        friend class component_mgr;

        friend const reflection::icomponent* dynamic_reflect(component*);

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

        REFLECT(component, MEM(m_enabled));
    };
}

