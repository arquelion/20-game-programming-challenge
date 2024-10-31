#pragma once
#include <algorithm>
#include <cmath>
#include <random>

extern FILE* logFile;
extern int frameCount;

const float FLOAT_INF = std::numeric_limits<float>::infinity();

using Radians = float;

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

inline glm::vec2 getUnitVector(Radians rad)
{
    return glm::vec2(cos(rad), sin(rad));
}

inline glm::vec2 norm(glm::vec2 u)
{
    return glm::vec2(-u.y, u.x) / glm::length(u);
}

inline Radians getAngleBetweenVectors(glm::vec2 u, glm::vec2 v)
{
    auto cos = std::clamp(glm::dot(u, v) / (glm::length(u) * glm::length(v)), -1.f, 1.f);
    auto angle = signum(glm::dot(norm(u), v)) * glm::acos(cos);
    if (std::isnan(angle))
    {
        throw std::exception("aaaaaaa");
    }
    return angle;
}
