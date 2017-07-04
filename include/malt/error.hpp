//
// Created by fatih on 4.07.2017.
//

#pragma once

#include <stdexcept>

namespace malt
{
    class undefined_component
            : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
}