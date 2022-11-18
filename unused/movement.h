#pragma once

#include <vector>

namespace board
{
    class movement
    {
    public:
        std::vector<unsigned int> from;
        std::vector<unsigned int> to;

        movement() = default;

        movement(const std::vector<unsigned int> &_from, const std::vector<unsigned int> &_to)
        {
            from = _from;
            to = _to;
        }
    };
}