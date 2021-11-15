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

    unsigned int get()
    {
        std::uniform_int_distribution<int> uid(_min, _max);
        return uid(_rng);
    }
};