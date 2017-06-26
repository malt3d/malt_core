//
// Created by Mehmet Fatih BAKIR on 26/06/2017.
//

#pragma once

#include <malt/engine_defs.hpp>
#include <ostream>
#include <malt/list.hpp>
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

    template <class ArT, class PrimitiveT,
            class = std::enable_if_t<std::is_fundamental<std::decay_t<PrimitiveT>>::value>>
    void serialize(ArT&& ar, const PrimitiveT& x)
    {
        ar = x;
    }

    template <class ArT, class T>
    void serialize(ArT&& ar, const T& t)
    {
        const component* c = &t; // invoke sfinae
        malt::meta::apply(T::reflect().members, [&](auto& member){
            /*std::ostringstream oss;
            oss << t.*(member.member);
            ar[member.name] = oss.str();*/
            serialize(ar[member.name], t.*(member.member));
        });
    }
}

namespace glm
{
    PUBLIC_REFLECT(vec3, MEM(x), MEM(y), MEM(z));

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