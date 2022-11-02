#pragma once

#include <random>

std::random_device rd;
std::mt19937_64 _rng(rd());

class rnd
{
private:
    unsigned int _min;
    unsigned int _max;

public:
    rnd(unsigned int min, unsigned int max)
    {
        _min = min;
        _max = max;
    }

    static unsigned int get(unsigned int min, unsigned int max)
    {
        std::uniform_int_distribution<int> uid(min, max);
        return uid(_rng);
    }

    unsigned int get()
    {
        return get(_min, _max);
    }
};