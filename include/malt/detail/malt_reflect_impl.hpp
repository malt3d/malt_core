//
// Created by fatih on 6/2/17.
//

#include <malt/entity.hpp>
#include <malt/serialization.hpp>
#include <cstring>

namespace malt
{
    namespace reflection
    {
        template <class comp_t>
        class component_type : public icomponent
        {
            mutable std::vector<const icomponent*> m_bases;
            comp_t_id m_component_index;

        public:
            void set_index(comp_t_id id)
            {
                m_component_index = id;
            }

            comp_t_id get_index() const override
            {
                return m_component_index;
            }

            const char* get_name() const override
            {
                return static_reflect(meta::type<comp_t>{}).name;
            }

            size_t get_type_hash() const override
            {
                return hash_c_string(get_name(), strlen(get_name()));
            }

            module_id get_module_id() const override
            {
                throw std::runtime_error("not implemented yet");
            }

            component* add_component(entity id) const override
            {
                return malt::add_component(get_type_hash(), id);
            }

            serialize_fun get_serialize_function() const override
            {
                return [](YAML::Node&& ar, component* c) {serialize(ar, *static_cast<comp_t*>(c));};
            }

            deserialize_fun get_deserialize_function() const override
            {
                return [](YAML::Node&& ar, component* c) {deserialize(ar, *static_cast<comp_t*>(c));};
            }

            icomponent_range get_base_components() const override
            {
                if (m_bases.empty())
                {
                    return {nullptr, nullptr};
                }
                return { &m_bases.front(), &m_bases.back() + 1 };
            }

            void add_base(const icomponent* ptr) const
            {
                m_bases.push_back(ptr);
            }
        };
    }
}

