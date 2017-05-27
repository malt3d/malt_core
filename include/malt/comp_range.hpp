//
// Created by fatih on 5/25/17.
//

#pragma once

#include <iterator>

namespace malt
{
    /*
     * this iterator is used to iterate over a malt::get_components<CompT> result
     * the components returned are possibly of a derived type
     */
    template <class BaseCompT>
    class component_iterator
            : public std::iterator<std::forward_iterator_tag, BaseCompT>
    {
    public:
    };
}

