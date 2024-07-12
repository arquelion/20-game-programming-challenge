#pragma once
#include "object2D.h"

class House : public Object2D
{
public:
    House(BoundedRect rect);

    void draw() const override;

    bool isOccupied = false;
};
