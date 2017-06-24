//
// Created by fatih on 6/2/17.
//

#pragma once

#include <cstddef>
#include <malt/malt_fwd.hpp>
#include <malt/list.hpp>
#include <malt/component_traits.hpp>
#include <yaml-cpp/yaml.h>

namespace malt {
    namespace reflection {

        class icomponent;

        struct icomponent_range
        {
            const icomponent** m_begin, ** m_end;
        public:
            const icomponent** begin() const
            { return m_begin; }

            const icomponent** end() const
            { return m_end; }
        };

        using serialize_fun = void (*) (YAML::Node&&, component*);

        class icomponent
        {
        public:
            virtual const char* get_name() const = 0;
            virtual size_t get_type_hash() const = 0;
            virtual module_id get_module_id() const = 0;
            virtual malt::component* add_component(entity_id id) const = 0;
            virtual icomponent_range get_base_components() const = 0;
            virtual serialize_fun get_serialize_function() const = 0;
            virtual ~icomponent() = default;
        };
    }
}

namespace malt {
    namespace reflection {

        struct nullmem{};

        struct not_nullmem
        {
            template <class T>
            static constexpr bool value()
            {
                return !std::is_same<T, nullmem>{};
            }
        };

        template<class>
        struct member_var_traits;
        template<class>
        struct member_fun_traits;

        template<class T, class Type>
        struct member_var_traits<T Type::*>
        {
            using variable_type = T;
            using reflected_type = Type;
        };

        template<class RetT, class TypeT, class... Args>
        struct member_fun_traits<RetT (TypeT::*)(Args...)>
        {
            using return_type = RetT;
            using reflected_type = TypeT;
            using arg_types = meta::list<Args...>;
        };

        template<class refl_type, class member_type>
        struct member_var_info
        {
            const char* name;
            member_type refl_type::* member;
        };

        template<class... Members>
        struct type_info
        {
            using member_ts = meta::list<Members...>;
            using tuple_t = meta::convert_t<std::tuple, member_ts>;

            const char* name;
            tuple_t members;
        };

        /*template <class T, class TypeT, class... Args>
        constexpr auto member(const char* name, T (TypeT::* x)(Args...))
        {
            using traits = member_fun_traits<decltype(x)>;
        };*/

        template<class T, class TypeT>
        constexpr auto member(const char* name, T TypeT::* t)
        {
            using traits = member_var_traits<decltype(t)>;
            return member_var_info<typename traits::reflected_type, typename traits::variable_type>{name, t};
        }

        constexpr auto type(const char* name, nullmem)
        {
            using t = type_info<>;
            return t{name, typename t::tuple_t{}};
        }

        template<class... members>
        constexpr auto type(const char* name, members... mems)
        {
            using t = type_info<members...>;
            return t{name, typename t::tuple_t{ mems... }};
        }
    }
}


#define MEM(var) \
    malt::reflection::member(#var, &refl_self_type::var)

#define NOMEM malt::reflection::nullmem {}

#define REFLECT(TYPE, ...) \
    using refl_self_type = TYPE;\
    static constexpr auto reflect() { \
        return malt::reflection::type(#TYPE, __VA_ARGS__); \
    }
