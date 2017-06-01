//
// Created by fatih on 6/2/17.
//

#include <malt/engine.hpp>

namespace malt
{
    namespace reflection
    {
        template <class comp_t>
        class component_type : public icomponent_type
        {
            mutable std::vector<const icomponent_type*> m_bases;

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

            component* add_component(entity_id id) const override
            {
                return malt::add_component(get_module_id(), id);
            }

            icomponent_range get_base_components() const override
            {
                if (m_bases.empty())
                {
                    return {nullptr, nullptr};
                }
                return { &m_bases.front(), &m_bases.back() + 1 };
            }

            void add_base(const icomponent_type* ptr) const
            {
                m_bases.push_back(ptr);
            }
        };
    }
}

