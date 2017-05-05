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
        };

        template <class CompT, class MsgT, class... Args>
        std::false_type dispatch_impl(std::false_type, CompT, MsgT, Args&&...)
        {
            return {};
        };
    }
    template <class CompT, class MsgT, class... ArgT>
    auto try_dispatch(CompT comp, MsgT, ArgT&& ... args)
    {
        using elem_t = typename std::iterator_traits<CompT>::value_type;
        return detail::dispatch_impl(can_handle<MsgT(ArgT...)>:: template value<elem_t>(),
                comp, MsgT{}, std::forward<ArgT>(args)...);
    };

    namespace detail {
        entity_id get_id(const entity& e);
    }
}
