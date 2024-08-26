#pragma once
class Car;

#include "collisionObjects.h"

class Car
{
public:
    Car();

    void loadTexture();

    void update(float deltaSec);
    void draw() const;

    Collideable2D object;
    float maxAccel = 10.f;
    Radians maxRotation = (float)M_PI / 4.f;
};
