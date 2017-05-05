//
// Created by fatih on 4/28/17.
//

#pragma once

#include <type_traits>

namespace malt
{
    template<class...>
    using void_t = void;

    template<class, template<class> class, class = void_t<>>
    struct detect : std::false_type { };

    template<class T, template<class> class Operation>
    struct detect<T, Operation, void_t<Operation<T>>> : std::true_type { };

    template <class U, class... Args>
    struct can_handle
    {
        template<class T>
        using hard_decay = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<T>>>;

        template<class T>
        using Value = decltype(std::declval<T>().Handle(std::declval<U>(), std::declval<Args>()...));

        template <class T>
        static constexpr auto value()
        {
            return detect<T, Value>();
        }
    };

    template <class U, class... Args>
    struct can_handle<U(Args...)> : public can_handle<U, Args...> {};
}

