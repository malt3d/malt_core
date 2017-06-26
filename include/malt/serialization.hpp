//
// Created by Mehmet Fatih BAKIR on 26/06/2017.
//

#pragma once

#include <malt/engine_defs.hpp>
#include <ostream>
#include <malt/track_ptr.hpp>
#include <malt/meta.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <sstream>
#include <type_traits>

namespace YAML
{
    class Node;
}

namespace malt
{
    void serialize(YAML::Node&& ar, entity e) MALT_PUBLIC;
    void serialize(YAML::Node& ar, entity e) MALT_PUBLIC;

    entity load_entity(YAML::Node& ar) MALT_PUBLIC;
    entity load_entity(YAML::Node&& ar) MALT_PUBLIC;

    template <class ArT, class PrimitiveT,
            class = std::enable_if_t<std::is_fundamental<std::decay_t<PrimitiveT>>::value>>
    void deserialize(ArT&& ar, PrimitiveT& x)
    {
        x = ar. template as<PrimitiveT>();
    };

    template <class ArT, class PrimitiveT,
            class = std::enable_if_t<std::is_fundamental<std::decay_t<PrimitiveT>>::value>>
    void serialize(ArT&& ar, const PrimitiveT& x)
    {
        ar = x;
    }

    template <class ArT, class CompT>
    void serialize(ArT&& ar, const track_ptr<CompT>& ptr)
    {
        ar["entity_id"] = ptr ?
                          detail::get_id(ptr->get_entity()) :
                          0;    // if the pointer is null, set id of entity to 0
                                // could leave it empty, but setting manually in text
                                // is cumbersome
        ar["comp_name"] = static_reflect(meta::type<CompT>{}).name;
    };

    template <class ArT, class CompT>
    void deserialize(ArT&& ar, track_ptr<CompT>& ptr)
    {
        throw std::runtime_error("it's not implemented yet :(");
    };

    template <class ArT, class T>
    std::enable_if_t<!std::is_fundamental<std::decay_t<T>>::value>
    deserialize(ArT&& ar, T& t)
    {
        malt::meta::apply(static_reflect(meta::type<T>{}).members, [&](auto& member){
            deserialize(ar[member.name], t.*(member.member));
        });
    }

    template <class ArT, class T>
    std::enable_if_t<!std::is_fundamental<std::decay_t<T>>::value>
    serialize(ArT&& ar, const T& t)
    {
        malt::meta::apply(static_reflect(meta::type<T>{}).members, [&](auto& member){
            serialize(ar[member.name], t.*(member.member));
        });
    }
}

namespace glm
{
    PUBLIC_REFLECT(vec2, MEM(x), MEM(y));
    PUBLIC_REFLECT(vec3, MEM(x), MEM(y), MEM(z));
    PUBLIC_REFLECT(quat, MEM(w), MEM(x), MEM(y), MEM(z));

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