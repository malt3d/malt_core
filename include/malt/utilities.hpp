//
// Created by Mehmet Fatih BAKIR on 29/04/2017.
//

#pragma once

#include <malt/malt_fwd.hpp>
#include <type_traits>
#include "component_traits.hpp"
#include <malt/engine_defs.hpp>
#include <ostream>
#include <malt/list.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sstream>

namespace YAML
{
    class Node;
}

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
        MALT_PUBLIC entity_id get_id(const entity& e);

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

    inline size_t hash_c_string(const char* p, size_t s) {
        size_t result = 0;
        const size_t prime = 31;
        for (size_t i = 0; i < s; ++i) {
            result = p[i] + (result * prime);
        }
        return result;
    }

    void serialize(YAML::Node&& ar, entity e) MALT_PUBLIC;
    void serialize(YAML::Node& ar, entity e) MALT_PUBLIC;

    template <class ArT, class T>
    void serialize(ArT&& ar, const T& t)
    {
        const component* c = &t; // invoke sfinae
        malt::meta::apply(T::reflect().members, [&](auto& member){
            std::ostringstream oss;
            oss << t.*(member.member);
            ar[member.name] = oss.str();
        });
    }
}

namespace glm
{
    inline std::ostream& operator<<(std::ostream& os, const vec3& v)
    {
        return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    }
    inline std::ostream& operator<<(std::ostream& os, const vec2& v)
    {
        return os << '(' << v.x << ", " << v.y << ')';
    }
    inline std::ostream& operator<<(std::ostream& os, const quat& q)
    {
        return os << '(' << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ')';
    }
}

