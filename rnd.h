#pragma once

#include <random>

std::random_device rd;
std::mt19937_64 _rng(rd());
std::uniform_int_distribution<long long unsigned int> uid(0, UINT64_MAX);

class rnd
{
public:
    inline static unsigned int get(unsigned int min, unsigned int max)
    {
        return min + uid(_rng) % (max - min + 1);
    }
};