#pragma once

#include <random>

std::random_device rd;
std::mt19937_64 _rng(rd());

class rnd
{
public:
    inline static unsigned int get(unsigned int min, unsigned int max)
    {
        std::uniform_int_distribution<int> uid(min, max);
        return uid(_rng);
    }
};