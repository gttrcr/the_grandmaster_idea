#pragma once

namespace board
{
    class movement2d
    {
    public:
        unsigned int from_x;
        unsigned int from_y;
        unsigned int to_x;
        unsigned int to_y;

        movement2d() = default;

        movement2d(const unsigned int &_from_x, const unsigned int &_from_y, const unsigned int &_to_x, const unsigned int &_to_y)
        {
            from_x = _from_x;
            from_y = _from_y;
            to_x = _to_x;
            to_y = _to_y;
        }
    };
}