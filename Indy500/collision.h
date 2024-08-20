#pragma once

#include "intersection.h"

enum class CollisionObject
{
    NONE,
    WALL,
    CAR,
};

struct Intersect
{
    Sweep closest;
    CollisionObject object;
};
