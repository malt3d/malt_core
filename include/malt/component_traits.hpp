//
// Created by fatih on 4/28/17.
//

#pragma once

#include <type_traits>
#include <malt/engine.hpp>

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

    template <class T>
    struct component_name
    {
        static const char* name;
        static size_t hash;
    };

    namespace reflection
    {
        class icomponent_type
        {
        public:
            virtual const char* get_name() const = 0;
            virtual size_t get_type_hash() const = 0;
            virtual module_id get_module_id() const = 0;
            virtual malt::component* add_component(entity_id id) = 0;
            virtual ~icomponent_type() = default;
        };

        template <class comp_t>
        class component_type : public icomponent_type
        {
        public:
            const char* get_name() const override
            {
                return component_name<comp_t>::name;
            }

            size_t get_type_hash() const override
            {
                return component_name<comp_t>::hash;
            }

            module_id get_module_id() const override
            {
                throw std::runtime_error("not implemented yet");
            }

            component* add_component(entity_id id) override
            {
                return malt::add_component(get_module_id(), id);
            }
        };
    }
}

