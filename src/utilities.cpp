//
// Created by fatih on 6/21/17.
//

#include <malt/utilities.hpp>
#include <malt/engine.hpp>
#include <malt/entity.hpp>
#include <yaml-cpp/yaml.h>
#include <malt/malt_reflect.hpp>
#include <malt/serialization.hpp>

namespace malt
{
    void serialize(YAML::Node& ar, entity e)
    {
        ar["name"] = e.get_name();
        for (auto comp : malt::impl::components_of(e))
        {
            auto&& refl = dynamic_reflect(comp);
            ar["components"][refl->get_name()] = YAML::Node{};
            refl->get_serialize_function()(ar["components"][refl->get_name()], comp);
        }
    }

    void serialize(YAML::Node&& ar, entity e)
    {
        serialize(ar, e);
    }

    entity load_entity(YAML::Node& ar)
    {
        entity e = malt::create_entity(ar["name"].as<std::string>());
        for (auto&& comp : ar["components"])
        {
            auto name = comp.first.as<std::string>();
            YAML::Node val = comp.second;
            component* c = malt::get_component(name.c_str(), e);
            if (!c)
            {
                c = malt::add_component(name.c_str(), e);
            }
            auto&& refl = dynamic_reflect(c);
            refl->get_deserialize_function()(std::move(val), c);
        }
        return e;
    }

    entity load_entity(YAML::Node&& ar)
    {
        return load_entity(ar);
    }
}
