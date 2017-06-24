//
// Created by fatih on 6/20/17.
//

#include <vector>
#include <string>
#include <malt/malt_fwd.hpp>
#include <bitset>
#include <stdexcept>
#include <algorithm>

namespace malt
{
namespace detail
{
    template <long comp_count>
    struct entity_manager
    {
        struct entity_details
        {
            entity_id id;
            std::string name;
            std::bitset<comp_count> components;

            operator entity_id()
            {
                return id;
            }
        };

        std::vector<entity_details> m_details;

        entity_details& get_details(entity_id id)
        {
            auto it = std::equal_range(m_details.begin(), m_details.end(), id);

            if (it.first == m_details.end())
            {
                throw std::runtime_error("entity not found");
            }
            return *it.first;
        }

        void add_entity(entity_id id, std::string name)
        {
            auto it = std::upper_bound(m_details.begin(), m_details.end(), id);
            m_details.insert(it, entity_details{ id, name });
        }

        void remove_entity(entity_id id)
        {
            auto it = std::lower_bound(m_details.begin(), m_details.end(), id);
            if (it == m_details.end())
            {
                throw std::runtime_error("entity not found");
            }
            m_details.erase(it);
        }

        void add_component(entity_id eid, comp_t_id cid)
        {
            get_details(eid).components[cid] = true;
        }

        void remove_component(entity_id eid, comp_t_id cid)
        {
            get_details(eid).components[cid] = false;
        }
    };
}
}
