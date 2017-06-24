//
// Created by fatih on 6/21/17.
//

#include <malt/utilities.hpp>
#include <malt/engine.hpp>
#include <malt/entity.hpp>
#include <yaml-cpp/yaml.h>
#include <malt/malt_reflect.hpp>

namespace malt
{
    void serialize(YAML::Node& ar, entity e)
    {
        for (auto comp : malt::impl::components_of(e))
        {
            auto&& refl = reflect(comp);
            refl->get_serialize_function()(ar[refl->get_name()], comp);
        }
    }

    void serialize(YAML::Node&& ar, entity e)
    {
        for (auto comp : malt::impl::components_of(e))
        {
            auto&& refl = reflect(comp);
            refl->get_serialize_function()(ar[refl->get_name()], comp);
        }
    }
}
