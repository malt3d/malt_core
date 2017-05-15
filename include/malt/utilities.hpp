//
// Created by Mehmet Fatih BAKIR on 29/04/2017.
//

#pragma once

#include <type_traits>
#include "component_traits.hpp"

namespace malt
{
    namespace detail
    {
        template <class CompT, class MsgT, class... Args>
        std::true_type dispatch_impl(std::true_type, CompT c, MsgT, Args&&... args)
        {
            c->Handle(MsgT{}, std::forward<Args>(args)...);
            return {};
        }

        template <class CompT, class MsgT, class... Args>
        std::false_type dispatch_impl(std::false_type, CompT, MsgT, Args&&...)
        {
            return {};
        }
    }

    template <class CompT, class MsgT, class... ArgT>
    auto try_dispatch(CompT comp, MsgT, ArgT&& ... args)
    {
        using elem_t = typename std::iterator_traits<CompT>::value_type;
        return detail::dispatch_impl(can_handle<MsgT(ArgT...)>:: template value<elem_t>(),
                comp, MsgT{}, std::forward<ArgT>(args)...);
    }

    namespace detail {
        entity_id get_id(const entity& e);
        template <class T>
        class at_exit
        {
            bool dismiss = false;
            T t;
        public:
            explicit at_exit(T&& t) : t(std::forward<T>(t)) {}

            at_exit(const T&) = delete;

            at_exit(at_exit&& rhs) :
                t(std::move(rhs.t))
            {
                rhs.dismiss = true;
            }

            at_exit& operator=(const T&) = delete;
            at_exit& operator=(T&&) = delete;

            ~at_exit(){ if(dismiss) return; t(); }
        };
    }

    template <class T>
    auto at_exit(T&& t)
    {
        return detail::at_exit<T>(std::forward<T>(t));
    }
}
