#pragma once
#include <algorithm>
#include <cmath>
#include <random>

extern FILE* logFile;
extern int frameCount;

const float FLOAT_INF = std::numeric_limits<float>::infinity();

// Strict inequality
template<typename T>
bool isBetween(T n, T lower, T upper)
{
    return n > lower && n < upper;
}

template<typename F>
bool isEqual(F a, F b)
{
    if (fabs(a) > 1.f && fabs(b) > 1.f)
    {
        return fabs(a - b) < 0.00001 * std::min(a, b);
    }
    return fabs(a - b) < 0.00001;
}

template<typename T>
T signum(T n)
{
    return T(1 - 2 * std::signbit(n));
}

template<typename T>
T getRandomNum()
{
    static std::random_device expensiveRandomizer;
    static std::mt19937_64 generator{ expensiveRandomizer() };
    static std::uniform_real_distribution<T> distribution{ T{0}, T{1} };

    return distribution(generator);
}
